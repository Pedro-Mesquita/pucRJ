/**
 * Multiplicacao de uma matriz de floats por um escalar (r = scalar * m)
 * com Intel Intrinsics / AVX, processando 8 floats por iteracao.
 *
 * Um registrador AVX tem 256 bits = 8 floats de 32 bits. O escalar e
 * replicado nas 8 posicoes de um registrador uma unica vez (broadcast) e o
 * vetor `values` e percorrido sequencialmente, como na versao com
 * ponteiros, mas avancando 8 elementos por passo: 1 load, 1 mul e 1 store
 * vetoriais substituem 8 iteracoes escalares.
 *
 * Codigos de erro:
 *  - (1) quantidade de linhas diferentes entre m e r
 *  - (2) quantidade de colunas diferentes entre m e r
 *  - (3) m ou r nulos
 *  - (4) values de m ou r nulo
 */

#include "matrix_lib.h"
#include <stdio.h>
#include <immintrin.h>

/**
 * @brief Calcula r = scalar_value * m usando AVX (8 floats por operacao).
 *
 * Usa loadu/storeu (sem exigencia de alinhamento): em processadores atuais
 * nao ha penalidade quando o endereco esta alinhado a 32 bytes -- caso do
 * aligned_alloc(32) do programa de teste -- e a funcao continua correta
 * se a biblioteca for chamada com um malloc comum.
 *
 * rows e cols sao multiplos de 8, entao rows * cols tambem e e o loop
 * vetorial cobre todos os elementos; o loop escalar final so existe por
 * seguranca, para uma matriz fora dessa hipotese.
 *
 * @param scalar_value Escalar.
 * @param m Matriz de entrada.
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
        __m256 v_m = _mm256_loadu_ps(p_m);        /* 8 floats de m       */
        __m256 v_r = _mm256_mul_ps(v_m, v_scalar); /* 8 produtos de uma vez */
        _mm256_storeu_ps(p_r, v_r);               /* 8 floats em r       */
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

int matrix_matrix_mult(matrix *m1, matrix *m2, matrix *r) {
    return 1;
}
