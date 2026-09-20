#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define LEN 10000
#define QTDTHREADS 4
#define VALOR_INICIAL 4

/*
    Com threads nao e preciso memoria compartilhada (shmget/shmat):
    todas as threads do processo enxergam o mesmo espaco de enderecamento,
    entao uma variavel global ja e naturalmente compartilhada.
*/
typedef struct
{
    double tempo_worker[QTDTHREADS];
    long vetor[LEN];
} memoria_t;

static memoria_t mem;
static pthread_barrier_t largada;
static pthread_barrier_t chegada;

typedef struct
{
    int indice;
} arg_t;

int todas_posicoes_iguais(const long vetor[]);
void *opera_sobre_vetores(void *arg);

static double agora(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main()
{
    pthread_t tid[QTDTHREADS];
    arg_t args[QTDTHREADS];

    if (pthread_barrier_init(&largada, NULL, QTDTHREADS + 1) != 0 ||
        pthread_barrier_init(&chegada, NULL, QTDTHREADS + 1) != 0)
    {
        perror("Erro ao criar barreira");
        exit(1);
    }

    for (int i = 0; i < LEN; i++)
    {
        mem.vetor[i] = VALOR_INICIAL;
    }

    for (int i = 0; i < QTDTHREADS; i++)
    {
        args[i].indice = i;

        if (pthread_create(&tid[i], NULL, opera_sobre_vetores, &args[i]) != 0)
        {
            perror("Erro ao criar thread");
            exit(1);
        }
    }

    pthread_barrier_wait(&largada);
    double inicio = agora();

    pthread_barrier_wait(&chegada);
    double fim = agora();

    double decorrido = fim - inicio;

    for (int i = 0; i < QTDTHREADS; i++)
    {
        pthread_join(tid[i], NULL);
    }

    pthread_barrier_destroy(&largada);
    pthread_barrier_destroy(&chegada);

    int diferentes = todas_posicoes_iguais(mem.vetor);

    long minimo = mem.vetor[0];
    long maximo = mem.vetor[0];
    int qtd_diferentes = 0;

    for (int i = 0; i < LEN; i++)
    {
        if (mem.vetor[i] < minimo)
        {
            minimo = mem.vetor[i];
        }
        if (mem.vetor[i] > maximo)
        {
            maximo = mem.vetor[i];
        }
        if (mem.vetor[i] != mem.vetor[0])
        {
            qtd_diferentes++;
        }
    }

    /* Valor de cada posicao se as 8 passadas fossem serializadas */
    long esperado = VALOR_INICIAL + 2;
    for (int i = 0; i < QTDTHREADS; i++)
    {
        esperado *= 2;
    }
    esperado -= 2;

    printf("threads trabalhadoras   : %d\n", QTDTHREADS);
    printf("tamanho do vetor        : %d\n", LEN);
    printf("valor inicial           : %d\n", VALOR_INICIAL);
    printf("valor esperado          : %ld\n\n", esperado);

    printf("tempo de calculo        : %.6f s (nao inclui create nem join)\n", decorrido);
    for (int i = 0; i < QTDTHREADS; i++)
    {
        printf("  worker %d              : %.6f s\n", i, mem.tempo_worker[i]);
    }
    printf("\n");

    printf("vetor[0]                : %ld\n", mem.vetor[0]);
    printf("menor valor             : %ld\n", minimo);
    printf("maior valor             : %ld\n", maximo);
    printf("posicoes != vetor[0]    : %d de %d\n\n", qtd_diferentes, LEN);

    if (diferentes == 0)
    {
        printf("VERIFICACAO: todas as %d posicoes tem o MESMO valor (%ld).\n", LEN, mem.vetor[0]);
        if (mem.vetor[0] == esperado)
        {
            printf("             e esse valor e o esperado (%ld): as 8 passadas\n", esperado);
            printf("             acabaram sendo serializadas nesta execucao.\n");
        }
        else
        {
            printf("             mas o valor difere do esperado (%ld): houve perda de\n", esperado);
            printf("             atualizacoes, so que de forma uniforme no vetor todo.\n");
        }
    }
    else
    {
        printf("VERIFICACAO: as posicoes NAO tem todas o mesmo valor.\n");
        printf("             %d das %d posicoes diferem de vetor[0].\n", qtd_diferentes, LEN);
        printf("             Faixa observada: [%ld, %ld].\n", minimo, maximo);
        printf("             Condicao de corrida: as threads compartilham o mesmo vetor\n");
        printf("             e a operacao vetor[j] = vetor[j]*2 + 2 nao e atomica\n");
        printf("             (le -> calcula -> escreve). Quando duas threads leem a\n");
        printf("             mesma posicao antes de qualquer escrita, uma atualizacao\n");
        printf("             e perdida (lost update), e o quanto se perde varia por\n");
        printf("             posicao conforme o escalonador intercala as threads.\n");
    }

    return 0;
}

int todas_posicoes_iguais(const long vetor[])
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

void *opera_sobre_vetores(void *arg)
{
    arg_t *a = (arg_t *)arg;
    double t0, t1;

    /* Fica pronta e espera a largada, para nao medir o pthread_create */
    pthread_barrier_wait(&largada);

    t0 = agora();

    for (int j = 0; j < LEN; j++)
    {
        mem.vetor[j] = (mem.vetor[j] * 2) + 2;
    }

    t1 = agora();
    mem.tempo_worker[a->indice] = t1 - t0;

    /* Avisa que terminou o calculo, antes de qualquer custo de encerramento */
    pthread_barrier_wait(&chegada);

    return NULL;
}
