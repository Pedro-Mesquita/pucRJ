#include "common.h"
#include <time.h>

static const struct timespec fatia = { 0, 10 * 1000 * 1000 };

/* Dorme 1 s de tempo de execução. Um sleep(1) direto conta tempo de relógio:
   se o app fica parado (SIGSTOP) no meio, o sleep "vence" enquanto ele está
   parado. Em fatias de 10 ms, o tempo parado conta no máximo 10 ms. */
static void dormir_1s(void)
{
    for (int i = 0; i < 100; i++)
        nanosleep(&fatia, NULL);
}

/* syscall(D1, op): pede a E/S ao kernel e espera ele liberar.
   Entre o kill e o SIGSTOP do kernel o app ainda roda; a espera na flag
   garante que ele não avança o PC nesse intervalo. */
static void syscall_d1(AppSlot *s, Operacao op)
{
    s->bloqueado = 1;                       /* liga antes do sinal: o kernel só desliga no IRQ1 */
    if (kill(getppid(), op == OP_READ ? SIG_SYS_R : SIG_SYS_W) == -1) {
        perror("app: kill");
        exit(1);
    }
    while (s->bloqueado)
        nanosleep(&fatia, NULL);
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        fprintf(stderr, "uso: %s <nome> <max> <shmid> <slot> [roteiro]\n", argv[0]);
        return 1;
    }

    if (atoi(argv[2]) <= 0)
    {
        fprintf(stderr, "MAX inválido: %s\n", argv[2]);
        return 1;
    }

    char *nome = argv[1];
    int max = atoi(argv[2]);
    int shmid = atoi(argv[3]);
    int slot = atoi(argv[4]);

    int pcs[MAX_SYSCALLS];
    Operacao ops[MAX_SYSCALLS];
    int nsys = ler_roteiro(argc > 5 ? argv[5] : "", max, pcs, ops);
    if (nsys == -1) {
        fprintf(stderr, "roteiro inválido: %s\n", argv[5]);
        return 1;
    }

    setvbuf(stdout, NULL, _IONBF, 0);

    AppSlot *slots = shmat(shmid, NULL, 0);
    if (slots == (void *)-1)
    {
        perror("app: shmat");
        return 1;
    }
    AppSlot *s = &slots[slot];

    /* O PC fica no slot, não numa variável local: assim o kernel consegue
       ler (salvar) e escrever (restaurar) o PC enquanto o app está parado.
       Cada iteração executa (PC++) e depois dorme 1 s ou faz a syscall do roteiro. */
    while (s->pc < max) {
        s->pc++;
        printf("[%s] PC=%d/%d\n", nome, s->pc, max);

        Operacao op = OP_NENHUMA;
        for (int k = 0; k < nsys; k++)
            if (pcs[k] == s->pc)
                op = ops[k];
        if (op != OP_NENHUMA) {
            printf("[%s] syscall(D1, %c) no PC=%d\n", nome, op, s->pc);
            syscall_d1(s, op);
            continue;       /* volta da E/S direto para a próxima iteração */
        }
        if (s->pc < max)
            dormir_1s();
    }

    shmdt(slots);
    return 0;
}
