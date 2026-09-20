/**
 * @file matrix_lib.c
 * @brief Biblioteca de operacoes aritmeticas com matrizes de floats,
 *        otimizada com Intel Intrinsics / AVX.
 *
 * Trabalho 1 - Introducao a Arquitetura de Computadores (PUC-Rio).
 *
 * Implementa as duas funcoes declaradas em matrix_lib.h:
 *
 *  - scalar_matrix_mult: r = scalar * m, percorrendo o vetor `values`
 *    sequencialmente e processando 8 floats por instrucao AVX.
 *
 *  - matrix_matrix_mult: r = m1 x m2 pelo algoritmo otimizado de somas
 *    parciais linha por linha (ordem i -> k -> j), com o loop interno
 *    vetorizado em blocos de 8 floats (_mm256_mul_ps + _mm256_add_ps).
 *
 * As matrizes sao row-major: o elemento (i, j) fica em values[i * cols + j].
 * rows e cols sao multiplos de 8, entao todo vetor de 8 floats cabe inteiro
 * numa linha; os loops escalares de "resto" existem apenas por seguranca.
 *
 * Compilacao (Linux / GCC):
 *   gcc -Wall -std=c11 -mfma -o matrix_lib_test matrix_lib_test.c matrix_lib.c
 *
 * Codigos de erro (as duas funcoes retornam 0 em caso de sucesso e imprimem
 * "[ERROR <linha>] ..." em stderr antes de retornar um codigo):
 *
 *   scalar_matrix_mult
 *     1  quantidade de linhas de m e r diferentes
 *     2  quantidade de colunas de m e r diferentes
 *     3  m ou r nulos
 *     4  values de m ou r nulo
 *
 *   matrix_matrix_mult
 *     1  colunas de m1 diferentes das linhas de m2 (produto indefinido)
 *     2  dimensoes de r incompativeis com m1 x m2
 *     3  m1, m2 ou r nulos
 *     4  values de m1, m2 ou r nulo
 *
 * As verificacoes acontecem na ordem 3, 4, 1, 2 (ponteiros antes de dimensoes).
 */

#include "matrix_lib.h"
#include <stdio.h>
#include <immintrin.h>

/**
 * @brief Multiplica cada elemento de uma matriz por um escalar usando AVX.
 *
 * O escalar e replicado nas 8 posicoes de um registrador de 256 bits uma
 * unica vez (broadcast). O vetor `values` e percorrido do inicio ao fim com
 * ponteiros que avancam 8 elementos por passo: 1 load, 1 mul e 1 store
 * vetoriais substituem 8 iteracoes escalares. Como a matriz e row-major e o
 * vetor e contiguo, a ordem de linhas/colunas nao importa aqui -- o acesso e
 * puramente sequencial.
 *
 * Usa loadu/storeu (sem exigencia de alinhamento): nao ha penalidade quando
 * o endereco esta alinhado a 32 bytes (aligned_alloc(32) do programa de
 * teste) e a funcao continua correta com um malloc comum.
 *
 * @param scalar_value Escalar.
 * @param m Matriz de entrada (rows x cols, row-major).
 * @param r Matriz resultado, com as mesmas dimensoes de m.
 *
 * @return 0 em caso de sucesso ou um dos codigos de erro do cabecalho.
 */
int scalar_matrix_mult(float scalar_value, matrix *m, matrix *r) {

    if (m == NULL || r == NULL) {
        fprintf(stderr, "[ERROR %d] Matriz de entrada ou de resposta nula\n", __LINE__);
        return 3;
    }

    if (m->values == NULL || r->values == NULL) {
        fprintf(stderr, "[ERROR %d] Values da matriz de entrada ou de resposta nulo\n", __LINE__);
        return 4;
    }

    if (m->rows != r->rows) {
        fprintf(stderr, "[ERROR %d] Matriz com quantidade de linhas distintas\n", __LINE__);
        return 1;
    }

    if (m->cols != r->cols) {
        fprintf(stderr, "[ERROR %d] Matriz com quantidade de colunas distintas\n", __LINE__);
        return 2;
    }

    unsigned long int n = m->rows * m->cols;
    float *p_m = m->values;
    float *p_r = r->values;

    /* escalar replicado nas 8 posicoes: [s, s, s, s, s, s, s, s] */
    __m256 v_scalar = _mm256_set1_ps(scalar_value);

    unsigned long int k;
    for (k = 0; k + 8 <= n; k += 8) {
        __m256 v_m = _mm256_loadu_ps(p_m);         /* 8 floats de m         */
        __m256 v_r = _mm256_mul_ps(v_m, v_scalar); /* 8 produtos de uma vez */
        _mm256_storeu_ps(p_r, v_r);                /* 8 floats em r         */
        p_m += 8;
        p_r += 8;
    }

    /* resto (n nao multiplo de 8): nunca executa com as matrizes do trabalho */
    for (; k < n; k++) {
        *p_r = *p_m * scalar_value;
        p_m++;
        p_r++;
    }

    return 0;
}

/**
 * @brief Multiplica duas matrizes (r = m1 x m2) por somas parciais em linha,
 *        com o loop interno vetorizado em AVX.
 *
 * Em vez de calcular cada r[i][j] como o produto escalar de uma linha de m1
 * por uma coluna de m2 (ordem i -> j -> k, que desce colunas de uma matriz
 * row-major e salta entre linhas de cache), a ordem e i -> k -> j: cada
 * m1[i][k] e replicado nas 8 posicoes de um registrador AVX e multiplicado
 * por 8 floats consecutivos da linha k de m2, acumulando em 8 floats
 * consecutivos da linha i de r. Assim m2 e r sao lidas e escritas
 * sequencialmente, aproveitando cada linha de cache por inteiro.
 *
 * A linha i de r e zerada antes de receber as p contribuicoes; nao ha
 * matrizes intermediarias.
 *
 * A acumulacao usa _mm256_mul_ps seguido de _mm256_add_ps (e nao FMA): sao
 * exatamente as mesmas operacoes, na mesma ordem e com os mesmos
 * arredondamentos da versao escalar, entao o resultado e identico bit a bit
 * ao do algoritmo convencional. Um FMA arredondaria uma unica vez e, em
 * elementos quase nulos por cancelamento, divergiria dessa referencia.
 *
 * Usa loadu/storeu pelo mesmo motivo de scalar_matrix_mult.
 *
 * @param m1 Matriz A (rows x cols).
 * @param m2 Matriz B (rows x cols), com m2->rows == m1->cols.
 * @param r  Matriz resultado, com r->rows == m1->rows e r->cols == m2->cols.
 *
 * @return 0 em caso de sucesso ou um dos codigos de erro do cabecalho.
 */
int matrix_matrix_mult(matrix *m1, matrix *m2, matrix *r) {

    if (m1 == NULL || m2 == NULL || r == NULL) {
        fprintf(stderr, "[ERROR %d] Matriz de entrada ou de resposta nula\n", __LINE__);
        return 3;
    }

    if (m1->values == NULL || m2->values == NULL || r->values == NULL) {
        fprintf(stderr, "[ERROR %d] Values da matriz de entrada ou de resposta nulo\n", __LINE__);
        return 4;
    }

    if (m1->cols != m2->rows) {
        fprintf(stderr, "[ERROR %d] Colunas de A (%lu) diferentes das linhas de B (%lu)\n",
            __LINE__, m1->cols, m2->rows);
        return 1;
    }

    if (r->rows != m1->rows || r->cols != m2->cols) {
        fprintf(stderr, "[ERROR %d] Matriz C (%lux%lu) incompativel com A x B (%lux%lu)\n",
            __LINE__, r->rows, r->cols, m1->rows, m2->cols);
        return 2;
    }

    unsigned long int n = m1->rows;   /* linhas de A e de C         */
    unsigned long int p = m1->cols;   /* colunas de A = linhas de B */
    unsigned long int q = m2->cols;   /* colunas de B e de C        */

    /* colunas cobertas pelos blocos de 8 (q inteiro, no trabalho) */
    unsigned long int q_vet = q - (q % 8);

    __m256 v_zero = _mm256_setzero_ps();

    for (unsigned long int i = 0; i < n; i++) {
        float *c_row = r->values + i * q;
        unsigned long int j;

        /* linha i de C comeca zerada: 8 zeros por store */
        for (j = 0; j < q_vet; j += 8) {
            _mm256_storeu_ps(c_row + j, v_zero);
        }
        for (; j < q; j++) {
            c_row[j] = 0.0f;
        }

        for (unsigned long int k = 0; k < p; k++) {
            const float *b_row = m2->values + k * q;

            /* A[i][k] replicado: [a, a, a, a, a, a, a, a] */
            __m256 v_a = _mm256_set1_ps(m1->values[i * p + k]);

            /* C[i][j..j+7] += A[i][k] * B[k][j..j+7]: 8 produtos, 8 somas */
            for (j = 0; j < q_vet; j += 8) {
                __m256 v_b = _mm256_loadu_ps(b_row + j);
                __m256 v_c = _mm256_loadu_ps(c_row + j);
                __m256 v_prod = _mm256_mul_ps(v_a, v_b);
                v_c = _mm256_add_ps(v_prod, v_c);
                _mm256_storeu_ps(c_row + j, v_c);
            }

            /* resto (q nao multiplo de 8): nunca executa com as matrizes do trabalho */
            for (; j < q; j++) {
                c_row[j] += m1->values[i * p + k] * b_row[j];
            }
        }
    }

    return 0;
}
