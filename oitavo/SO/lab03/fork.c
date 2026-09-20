#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sched.h>
#include <time.h>

#define LEN 10000
#define QTDFORKS 8
#define VALOR_INICIAL 4

typedef struct
{
    volatile int prontos;
    volatile int largada;
    volatile int terminados;
    double tempo_worker[QTDFORKS];
    int vetor[LEN];
} memoria_t;

int todas_posicoes_iguais(const int vetor[]);

static double agora(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

static void espera(void)
{
    sched_yield();
}

int main()
{
    memoria_t *mem;
    int id;

    id = shmget(IPC_PRIVATE, sizeof(memoria_t), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if (id == -1)
    {
        perror("shmget");
        exit(1);
    }

    mem = (memoria_t *)shmat(id, NULL, 0);
    if (mem == (memoria_t *)-1)
    {
        perror("shmat");
        shmctl(id, IPC_RMID, 0);
        exit(1);
    }

    /*
        Cria vetor com 10k posições e inicializa com 4
    */
    mem->prontos = 0;
    mem->largada = 0;
    mem->terminados = 0;

    for (int i = 0; i < LEN; i++)
    {
        mem->vetor[i] = VALOR_INICIAL;
    }

    /* Cria os 8 trabalhadores irmaos*/
    for (int i = 0; i < QTDFORKS; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("Erro ao criar processo");
            shmdt(mem);
            shmctl(id, IPC_RMID, 0);
            exit(1);
        }
        else if (pid == 0)
        {
            double t0, t1;

            __sync_fetch_and_add(&mem->prontos, 1);
            while (mem->largada == 0)
            {
                espera();
            }
            
            t0 = agora();

            for (int j = 0; j < LEN; j++)
            {
                mem->vetor[j] = (mem->vetor[j] * 2) + 2;
            }

            t1 = agora();
            mem->tempo_worker[i] = t1 - t0;

            __sync_fetch_and_add(&mem->terminados, 1);
            shmdt(mem);
            exit(0);
        }
    }

    while (mem->prontos < QTDFORKS)
    {
        espera();
    }

    double inicio = agora();
    mem->largada = 1;

    while (mem->terminados < QTDFORKS)
    {
        espera();
    }

    double fim = agora();
    double decorrido = fim - inicio;

    for (int i = 0; i < QTDFORKS; i++)
    {
        wait(NULL);
    }

    /*
        Verifica máximo, mínimo e qtd de diferentes
    */

    int diferentes = todas_posicoes_iguais(mem->vetor);

    int minimo = mem->vetor[0];
    int maximo = mem->vetor[0];
    int qtd_diferentes = 0;

    for (int i = 0; i < LEN; i++)
    {
        if (mem->vetor[i] < minimo)
        {
            minimo = mem->vetor[i];
        }
        if (mem->vetor[i] > maximo)
        {
            maximo = mem->vetor[i];
        }
        if (mem->vetor[i] != mem->vetor[0])
        {
            qtd_diferentes++;
        }
    }

    /* Valor de cada posicao se as 8 passadas fossem serializadas*/
    int esperado = VALOR_INICIAL + 2;
    for (int i = 0; i < QTDFORKS; i++)
    {
        esperado *= 2;
    }
    esperado -= 2;

    printf("processos trabalhadores : %d\n", QTDFORKS);
    printf("tamanho do vetor        : %d\n", LEN);
    printf("valor inicial           : %d\n", VALOR_INICIAL);
    printf("valor esperado : %d\n\n", esperado);

    printf("tempo de calculo        : %.6f s (nao inclui fork nem wait)\n", decorrido);
    for (int i = 0; i < QTDFORKS; i++)
    {
        printf("  worker %d              : %.6f s\n", i, mem->tempo_worker[i]);
    }
    printf("\n");

    printf("vetor[0]                : %d\n", mem->vetor[0]);
    printf("menor valor             : %d\n", minimo);
    printf("maior valor             : %d\n", maximo);
    printf("posicoes != vetor[0]    : %d de %d\n\n", qtd_diferentes, LEN);

    shmdt(mem);
    shmctl(id, IPC_RMID, 0);

    return 0;
}

int todas_posicoes_iguais(const int vetor[])
{
    for (int i = 1; i < LEN; i++)
    {
        if (vetor[i] != vetor[0])
        {
            return 1;
        }
    }

    return 0;
}
