#include "common.h"
#include <string.h>
#include <time.h>

typedef enum { READY, RUNNING, BLOCKED, FINISHED } Estado;

static const char *nome_estado[] = { "READY", "RUNNING", "BLOCKED", "FINISHED" };

/* PCB: tudo que o kernel precisa saber de um processo para tirá-lo e devolvê-lo à CPU. */
typedef struct {
    pid_t pid;
    char nome[8];
    Estado estado;
    int pc;     /* contexto salvo: PC no momento em que o processo perdeu a CPU */
    Operacao op;    /* contexto salvo: parâmetro da syscall pendente (OP_NENHUMA se nenhuma) */
    int max;
    char roteiro[64];
} PCB;

static PCB pcb[MAX_APPS];
static int napps = 0;
static int atual = -1;          /* índice do PCB em execução; -1 = CPU ociosa */

static AppSlot *slots;          /* memória compartilhada com os apps */
static int shmid = -1;
static pid_t ic = -1;           /* pid do InterController */

/* FIFO circular de índices de PCB. Usada para a fila de prontos e para a fila de D1. */
typedef struct {
    int v[MAX_APPS];
    int ini, tam;
} Fila;

static Fila prontos, d1;

static void push_back(Fila *f, int i)
{
    f->v[(f->ini + f->tam) % MAX_APPS] = i;
    f->tam++;
}

/* Entra na frente da fila: usado por quem volta da E/S (prioridade). */
static void push_front(Fila *f, int i)
{
    f->ini = (f->ini + MAX_APPS - 1) % MAX_APPS;
    f->v[f->ini] = i;
    f->tam++;
}

static int pop_front(Fila *f)
{
    if (f->tam == 0)
        return -1;
    int i = f->v[f->ini];
    f->ini = (f->ini + 1) % MAX_APPS;
    f->tam--;
    return i;
}

static int fila_get(const Fila *f, int k)
{
    return f->v[(f->ini + k) % MAX_APPS];
}

/* Tira i do meio da fila, mantendo a ordem dos outros. */
static void remover(Fila *f, int i)
{
    int n = f->tam;
    f->tam = 0;
    for (int k = 0; k < n; k++) {
        int j = f->v[(f->ini + k) % MAX_APPS];
        if (j != i)
            f->v[(f->ini + f->tam++) % MAX_APPS] = j;
    }
}

static volatile sig_atomic_t irq0_flag = 0;
static volatile sig_atomic_t irq1_flag = 0;
static volatile sig_atomic_t chld_flag = 0;
static volatile sig_atomic_t fim_flag = 0;

/* Syscalls que chegaram e ainda não foram tratadas. Sinais de tempo real
   enfileiram, então duas syscalls seguidas chegam as duas. */
static struct { pid_t pid; Operacao op; } pend[MAX_APPS];
static volatile sig_atomic_t npend = 0;

static void on_signal(int sig)
{
    if (sig == SIG_IRQ0)
        irq0_flag = 1;
    else if (sig == SIG_IRQ1)
        irq1_flag = 1;
    else if (sig == SIGCHLD)
        chld_flag = 1;
    else
        fim_flag = 1;
}

/* SA_SIGINFO: além do número do sinal, recebemos quem mandou (si_pid). */
static void on_syscall(int sig, siginfo_t *info, void *ctx)
{
    (void)ctx;
    if (npend < MAX_APPS) {
        pend[npend].pid = info->si_pid;
        pend[npend].op = (sig == SIG_SYS_R) ? OP_READ : OP_WRITE;
        npend++;
    }
}

static struct timespec t0;

static double agora(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (t.tv_sec - t0.tv_sec) + (t.tv_nsec - t0.tv_nsec) / 1e9;
}

/* Uma linha por evento: tempo, quem roda, fila de prontos, fila de D1 e PC de cada um.
   A linha é montada num buffer e escrita de uma vez, para o print dos apps não cair no meio. */
static void log_estado(const char *evento)
{
    char buf[512];
    int n = snprintf(buf, sizeof buf, "t=%4.1f %-8s| RUN=%-3s | READY=[", agora(), evento,
                     atual == -1 ? "--" : pcb[atual].nome);
    for (int k = 0; k < prontos.tam; k++)
        n += snprintf(buf + n, sizeof buf - n, "%s%s", k ? " " : "",
                      pcb[fila_get(&prontos, k)].nome);
    n += snprintf(buf + n, sizeof buf - n, "] | D1=[");
    for (int k = 0; k < d1.tam; k++) {
        int i = fila_get(&d1, k);
        n += snprintf(buf + n, sizeof buf - n, "%s%s:%c", k ? " " : "", pcb[i].nome, pcb[i].op);
    }
    n += snprintf(buf + n, sizeof buf - n, "] | PC:");
    for (int i = 0; i < napps; i++)
        n += snprintf(buf + n, sizeof buf - n, " %s=%d/%d%s", pcb[i].nome, pcb[i].pc,
                      pcb[i].max, pcb[i].estado == FINISHED ? "(F)" : "");
    snprintf(buf + n, sizeof buf - n, "\n");
    fputs(buf, stdout);
}

static int acha_pcb(pid_t pid)
{
    for (int i = 0; i < napps; i++)
        if (pcb[i].pid == pid)
            return i;
    return -1;
}

/* Para o processo i e salva o contexto dele.
   Retorna 1 se ele ficou parado, 0 se ele já tinha terminado. */
static int parar_processo(int i)
{
    int st;
    kill(pcb[i].pid, SIGSTOP);
    /* O SIGSTOP é assíncrono: só lemos o PC depois de ter certeza de que parou. */
    if (waitpid(pcb[i].pid, &st, WUNTRACED) == -1)
        perror("kernel: waitpid (parar_processo)");
    pcb[i].pc = slots[i].pc;                /* salva o contexto */
    if (WIFSTOPPED(st))
        return 1;
    pcb[i].estado = FINISHED;
    return 0;
}

/* Tira o próximo da fila de prontos e entrega a CPU para ele. */
static void escalonar(void)
{
    atual = pop_front(&prontos);
    if (atual == -1)
        return;                             /* CPU ociosa */
    slots[atual].pc = pcb[atual].pc;        /* restaura o contexto */
    pcb[atual].estado = RUNNING;
    kill(pcb[atual].pid, SIGCONT);
}

static int todos_terminaram(void)
{
    for (int i = 0; i < napps; i++)
        if (pcb[i].estado != FINISHED)
            return 0;
    return 1;
}

/* Recolhe os apps que terminaram (evita zumbis) e, se era o atual, escalona outro. */
static void tratar_sigchld(void)
{
    int st;
    pid_t pid;
    while ((pid = waitpid(-1, &st, WNOHANG)) > 0) {
        if (pid == ic) {
            fprintf(stderr, "kernel: InterController morreu\n");
            ic = -1;
            continue;
        }
        int i = acha_pcb(pid);
        if (i == -1)
            continue;
        pcb[i].pc = slots[i].pc;
        pcb[i].estado = FINISHED;
        if (i == atual) {
            atual = -1;
            escalonar();
        }
        log_estado("FIM");
        if (todos_terminaram())
            printf("kernel: todos os processos terminaram (Ctrl+C para sair)\n");
    }
}

static void tratar_irq0(void)
{
    if (atual != -1) {
        int i = atual;
        atual = -1;
        if (parar_processo(i)) {
            pcb[i].estado = READY;
            push_back(&prontos, i);         /* perdeu a CPU por time slice: fim da fila */
        }
    }
    escalonar();
    log_estado("IRQ0");
}

/* syscall(D1, op) do processo pid: bloqueia na hora e entrega a CPU para o próximo. */
static void tratar_syscall(pid_t pid, Operacao op)
{
    int i = acha_pcb(pid);
    if (i == -1 || pcb[i].estado == BLOCKED || pcb[i].estado == FINISHED)
        return;

    if (i == atual) {
        atual = -1;
        if (!parar_processo(i)) {           /* terminou antes de ser parado */
            escalonar();
            log_estado("FIM");
            return;
        }
    } else {
        /* Um IRQ0 tratado antes desta syscall já parou o processo e o pôs
           nos prontos. Ele já está parado: basta tirá-lo da fila. */
        remover(&prontos, i);
    }

    pcb[i].op = op;                         /* salva o parâmetro da syscall no contexto */
    pcb[i].estado = BLOCKED;
    push_back(&d1, i);                      /* fila de bloqueados de D1: FIFO */
    if (ic > 0)
        kill(ic, SIG_IO_REQ);               /* avisa o controlador: começou um pedido em D1 */
    if (atual == -1)
        escalonar();
    log_estado(op == OP_READ ? "SYS(R)" : "SYS(W)");
}

/* IRQ1: terminou a E/S do primeiro da fila de D1. */
static void tratar_irq1(void)
{
    int i = pop_front(&d1);
    if (i == -1)
        return;
    char evento[16];
    snprintf(evento, sizeof evento, "IRQ1(%c)", pcb[i].op);
    pcb[i].op = OP_NENHUMA;                 /* a syscall terminou: sai do contexto */
    slots[i].bloqueado = 0;                 /* libera o app da espera em syscall_d1 */
    pcb[i].estado = READY;
    push_front(&prontos, i);                /* prioridade sobre quem saiu por time slice */
    if (atual == -1)
        escalonar();                        /* CPU estava ociosa: ele roda na hora */
    log_estado(evento);
}

static void encerrar(void)
{
    if (ic > 0) {
        kill(ic, SIGKILL);
        waitpid(ic, NULL, 0);
    }
    for (int i = 0; i < napps; i++) {
        if (pcb[i].estado != FINISHED) {
            kill(pcb[i].pid, SIGKILL);
            waitpid(pcb[i].pid, NULL, 0);
        }
    }
    shmdt(slots);
    shmctl(shmid, IPC_RMID, NULL);
    printf("kernel: encerrado\n");
}

int main(int argc, char *argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0);

    napps = argc - 1;
    if (napps < 3 || napps > MAX_APPS) {
        fprintf(stderr, "uso: %s <MAX_A1> <MAX_A2> <MAX_A3> [... até A%d]\n", argv[0], MAX_APPS);
        return 1;
    }
    /* Cada argumento é MAX[:roteiro], por exemplo 8:3R,6W. */
    for (int i = 0; i < napps; i++) {
        pcb[i].max = atoi(argv[i + 1]);
        if (pcb[i].max <= 0) {
            fprintf(stderr, "MAX inválido: %s\n", argv[i + 1]);
            return 1;
        }
        char *dois_pontos = strchr(argv[i + 1], ':');
        snprintf(pcb[i].roteiro, sizeof pcb[i].roteiro, "%s", dois_pontos ? dois_pontos + 1 : "");
        int pcs[MAX_SYSCALLS];
        Operacao ops[MAX_SYSCALLS];
        if (ler_roteiro(pcb[i].roteiro, pcb[i].max, pcs, ops) == -1) {
            fprintf(stderr, "roteiro inválido em %s (use PC entre 1 e MAX seguido de R ou W, ex.: 3R,7W)\n",
                    argv[i + 1]);
            return 1;
        }
    }

    /* Bloqueia os sinais tratados: eles só são entregues dentro do sigsuspend.
       Isso evita perder um sinal que chega entre o teste da flag e a espera. */
    sigset_t bloq, orig;
    sigemptyset(&bloq);
    sigaddset(&bloq, SIG_IRQ0);
    sigaddset(&bloq, SIG_IRQ1);
    sigaddset(&bloq, SIG_SYS_R);
    sigaddset(&bloq, SIG_SYS_W);
    sigaddset(&bloq, SIGCHLD);
    sigaddset(&bloq, SIGINT);
    sigaddset(&bloq, SIGTERM);
    sigprocmask(SIG_BLOCK, &bloq, &orig);

    struct sigaction sa = {0};
    sa.sa_handler = on_signal;
    sa.sa_mask = bloq;          /* um handler não interrompe outro no meio */
    sa.sa_flags = 0;
    if (sigaction(SIG_IRQ0, &sa, NULL) == -1 ||
        sigaction(SIG_IRQ1, &sa, NULL) == -1 ||
        sigaction(SIGINT, &sa, NULL) == -1 ||
        sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("kernel: sigaction");
        return 1;
    }
    /* SA_NOCLDSTOP: sem isso, cada SIGSTOP num app geraria um SIGCHLD. */
    sa.sa_flags = SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("kernel: sigaction SIGCHLD");
        return 1;
    }
    sa.sa_sigaction = on_syscall;
    sa.sa_flags = SA_SIGINFO;
    if (sigaction(SIG_SYS_R, &sa, NULL) == -1 ||
        sigaction(SIG_SYS_W, &sa, NULL) == -1) {
        perror("kernel: sigaction syscall");
        return 1;
    }

    shmid = shmget(IPC_PRIVATE, sizeof(AppSlot) * MAX_APPS, IPC_CREAT | 0600);
    if (shmid == -1) {
        perror("kernel: shmget");
        return 1;
    }
    slots = shmat(shmid, NULL, 0);
    if (slots == (void *)-1) {
        perror("kernel: shmat");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    clock_gettime(CLOCK_MONOTONIC, &t0);
    printf("kernel pid=%d shmid=%d\n", getpid(), shmid);

    /* Cria os apps antes do InterController: o primeiro IRQ0 já encontra os PCBs prontos. */
    for (int i = 0; i < napps; i++) {
        snprintf(pcb[i].nome, sizeof pcb[i].nome, "A%d", i + 1);
        pcb[i].pc = 0;
        pcb[i].op = OP_NENHUMA;
        slots[i].pc = 0;
        slots[i].bloqueado = 0;

        pid_t pid = fork();
        if (pid == -1) {
            perror("kernel: fork app");
            encerrar();
            return 1;
        }
        if (pid == 0) {
            char s_max[16], s_shm[16], s_slot[16];
            snprintf(s_max, sizeof s_max, "%d", pcb[i].max);
            snprintf(s_shm, sizeof s_shm, "%d", shmid);
            snprintf(s_slot, sizeof s_slot, "%d", i);
            sigprocmask(SIG_SETMASK, &orig, NULL);  /* a máscara é herdada; o app não deve herdá-la */
            raise(SIGSTOP);                          /* nasce parado: só roda quando for escalonado */
            execl("./app", "app", pcb[i].nome, s_max, s_shm, s_slot, pcb[i].roteiro, (char *)NULL);
            perror("kernel: exec app");
            _exit(1);
        }

        int st;
        waitpid(pid, &st, WUNTRACED);               /* confirma que o filho já está parado */
        pcb[i].pid = pid;
        pcb[i].estado = READY;
        push_back(&prontos, i);
        printf("kernel: %s pid=%d criado (%s, MAX=%d, roteiro=%s)\n", pcb[i].nome, pid,
               nome_estado[pcb[i].estado], pcb[i].max, pcb[i].roteiro[0] ? pcb[i].roteiro : "-");
    }

    ic = fork();
    if (ic == -1) {
        perror("kernel: fork intercontroller");
        encerrar();
        return 1;
    }
    if (ic == 0) {
        /* O aviso de E/S já nasce bloqueado (a máscara passa pelo exec): se um app
           pedir E/S antes de o InterController terminar de iniciar, a ação padrão
           do sinal (matar) não chega a acontecer. */
        sigset_t m = orig;
        sigaddset(&m, SIG_IO_REQ);
        sigprocmask(SIG_SETMASK, &m, NULL);
        execl("./intercontroller", "intercontroller", (char *)NULL);
        perror("kernel: exec intercontroller");
        _exit(1);
    }
    printf("kernel: intercontroller pid=%d\n", ic);

    escalonar();
    log_estado("INICIO");

    sigset_t espera = orig;
    sigdelset(&espera, SIG_IRQ0);
    sigdelset(&espera, SIG_IRQ1);
    sigdelset(&espera, SIG_SYS_R);
    sigdelset(&espera, SIG_SYS_W);
    sigdelset(&espera, SIGCHLD);
    sigdelset(&espera, SIGINT);
    sigdelset(&espera, SIGTERM);

    while (!fim_flag) {
        sigsuspend(&espera);    /* dorme até chegar um sinal (o kernel é "dirigido a interrupções") */
        if (chld_flag) {
            chld_flag = 0;
            tratar_sigchld();
        }
        /* Syscalls antes do IRQ0: se o IRQ0 viesse primeiro, quem pediu E/S
           voltaria para a fila de prontos em vez de bloquear. */
        for (int k = 0; k < npend; k++)
            tratar_syscall(pend[k].pid, pend[k].op);
        npend = 0;
        /* IRQ1 antes do IRQ0: quem sai da E/S entra na frente da fila e
           o IRQ0 logo em seguida já entrega a CPU para ele. */
        if (irq1_flag) {
            irq1_flag = 0;
            tratar_irq1();
        }
        if (irq0_flag) {
            irq0_flag = 0;
            tratar_irq0();
        }
    }

    encerrar();
    return 0;
}
