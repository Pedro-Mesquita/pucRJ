#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define QTDFORKS 6
#define QUANTUM 100000
#define MAXFATIAS 5

typedef struct
{
    pid_t pid;
    int fatias;
} Processo;

typedef struct
{
    Processo processos[QTDFORKS];
    int inicio;
    int fim;
    int quantidade;
} Fila;

void inicializaFila(Fila *fila);
void enfilerar(Fila *fila, Processo p);
Processo desenfileirar(Fila *fila);
void finalizaProcesso(Processo p);

int main()
{
    Fila fila;
    inicializaFila(&fila);

    for (int i = 0; i < QTDFORKS; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("Erro ao criar processo");
            exit(1);
        }
        else if (pid == 0)
        {
            raise(SIGSTOP);

            while (1)
            {
                printf("printando o prog %d\n", getpid());
                fflush(stdout);
                usleep(10000);
            }
        }
        else
        {
            int status;
            waitpid(pid, &status, WUNTRACED);

            if (WIFSTOPPED(status))
            {
                Processo p = {pid, 0};
                enfilerar(&fila, p);
            }
        }
    }

    while (fila.quantidade > 0)
    {
        Processo p = desenfileirar(&fila);

        kill(p.pid, SIGCONT);
        usleep(QUANTUM);
        kill(p.pid, SIGSTOP);

        int status;
        waitpid(p.pid, &status, WUNTRACED);

        p.fatias++;

        if (p.fatias >= MAXFATIAS)
        {
            finalizaProcesso(p);
        }
        else if (WIFSTOPPED(status))
        {
            enfilerar(&fila, p);
        }
    }

    printf("Todos os processos foram finalizados\n");
    return 0;
}

void finalizaProcesso(Processo p)
{
    printf("Processo %d atingiu %d fatias, finalizando\n", p.pid, p.fatias);
    kill(p.pid, SIGKILL);
    waitpid(p.pid, NULL, 0);
}

void enfilerar(Fila *fila, Processo p)
{
    if (fila->quantidade == QTDFORKS)
        return;

    fila->processos[fila->fim] = p;
    fila->fim = (fila->fim + 1) % QTDFORKS;
    fila->quantidade++;
}

Processo desenfileirar(Fila *fila)
{
    Processo p = fila->processos[fila->inicio];
    fila->inicio = (fila->inicio + 1) % QTDFORKS;
    fila->quantidade--;

    return p;
}

void inicializaFila(Fila *fila)
{
    fila->inicio = 0;
    fila->fim = 0;
    fila->quantidade = 0;
}
