/**
 * Programa para multiplicar 2 matrizes de doubles
 * 
 * O programa deverá ler as matrizes de um arquivo BINÁRIO
 * As dimensões das matrizes devem ser obtidas pela
 * linha de comando
 * Argumentos da linha de comando:
 * - (1) nome do arquivo com a 1a matriz
 * - (2) número de linha da 1a matriz
 * - (3) número de colunas da 1a matriz
 * - (4) nome do arquivo com a 2a matriz
 * - (5) número de linha da 2a matriz
 * - (6) número de colunas da 2a matriz
 * - (7) nome do arquivo com a matriz-resposta
 * obs.: argv[0] é o nome do programa
 * 
 * Códigos de erro:
 * - 1: erro ao alocar memória para a matriz A
 * - 2: erro ao alocar memória para a matriz B
 * - 3: erro ao alocar memória para a matriz C
 * - 4: erro ao abrir arquivo binário para leitura
 * - 5: arquivo trucado
 * - 6: erro ao abrir arquivo binário para escrita
 * - 7: erro ao escrever o arquivo
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct s_matriz {
    int n_linhas;
    int n_colunas;
    double *p_matriz;
} t_matriz;

#define valor(matriz, linha, coluna) (*((matriz).p_matriz + (linha) * (matriz).n_colunas + (coluna)))

void le_matriz(char *nome, t_matriz *matriz);
void salva_matriz(char *nome, t_matriz *matriz);

int main(int argc, char *argv[]) {
    t_matriz a;
    t_matriz b;
    t_matriz c;

    // Leitura dos dados da matriz A
    a.n_linhas = atoi(argv[2]);
    a.n_colunas = atoi(argv[3]);
    // trocar depois para aligned_alloc
    a.p_matriz = (double *)malloc(a.n_linhas * a.n_colunas * sizeof(double));
    if(!a.p_matriz) {
        fprintf(stderr, "[ERROR %d] Erro ao alocar memória para a matriz A\n", __LINE__);
        exit(1);
    }
    le_matriz(argv[1], &a);

    // Leitura dos dados da matriz B
    b.n_linhas = atoi(argv[5]);
    b.n_colunas = atoi(argv[6]);
    // trocar depois para aligned_alloc
    b.p_matriz = (double *)malloc(b.n_linhas * b.n_colunas * sizeof(double));
    if(!b.p_matriz) {
        fprintf(stderr, "[ERROR %d] Erro ao alocar memória para a matriz B\n", __LINE__);
        exit(2);
    }
    le_matriz(argv[4], &b);

    // Criar a matriz C
    c.n_linhas = a.n_linhas;
    c.n_colunas = b.n_colunas;
    // trocar depois para aligned_alloc
    c.p_matriz = (double *)malloc(c.n_linhas * c.n_colunas * sizeof(double));
    if(!c.p_matriz) {
        fprintf(stderr, "[ERROR %d] Erro ao alocar memória para a matriz C\n", __LINE__);
        exit(3);
    }

    // Cálculo da matriz C
    for(int i=0; i<c.n_linhas; i++) {
        for(int j=0; j<c.n_colunas; j++) {
            // C[i][j] = 0
            valor(c, i, j) = 0;
            for(int k=0; k<a.n_colunas; k++) {
                // C[i][j] += A[i][k] * B[k][j]
                valor(c, i, j) += valor(a, i, k) * valor(b, k, j);
            }
        }
    }

    salva_matriz(argv[7], &c);

    return 0;
}


/**
 * le_matriz
 * 
 * Lê o arquivo <nome> uma matriz binária de doubles
 * 
 * @param nome nome do arquivo
 * @param matriz ponteiro para estrutura que representa a matriz
 * @return void
 */
void le_matriz(char *nome, t_matriz *matriz) {
    FILE *arq;
    int n;

    arq = fopen(nome, "rb");
    if(!arq) {
        fprintf(stderr, "[ERROR %d] Não foi possível abrir o arquivo %s para leitura\n", 
            __LINE__,  nome);
        exit(4);
    }
    n = fread(matriz->p_matriz, sizeof(double), matriz->n_linhas * matriz->n_colunas, arq);
    if(n != matriz->n_linhas * matriz->n_colunas) {
        fprintf(stderr, "[ERROR %d] Erro no tamanho da matriz %s (%d != %d)\n", 
            __LINE__, nome, n, matriz->n_linhas * matriz->n_colunas);
        exit(5);
    }
    fclose(arq);
}

/**
 * salva_matriz
 * 
 * Salva uma matriz binária de doubles no arquivo <nome>
 * 
 * @param nome nome do arquivo
 * @param matriz ponteiro para a estrutura da matriz
 * @return void
 */
void salva_matriz(char *nome, t_matriz *matriz) {
    FILE *arq;
    int n;

    arq = fopen(nome, "wb");
    if(!arq) {
        fprintf(stderr, "[ERROR %d] Não foi possível abrir o arquivo %s para escrita\n", 
            __LINE__,  nome);
        exit(6);
    }
    n = fwrite(matriz->p_matriz, sizeof(double), matriz->n_linhas * matriz->n_colunas, arq);
    if(n != matriz->n_linhas * matriz->n_colunas) {
        fprintf(stderr, "[ERROR %d] Erro ao escrever a matriz %s (%d != %d)\n", 
            __LINE__, nome, n, matriz->n_linhas * matriz->n_colunas);
        exit(7);
    }
    fclose(arq);
}