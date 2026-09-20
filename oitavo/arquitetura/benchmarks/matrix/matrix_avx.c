/**
 * Multiplicacao de matrizes de floats (C = A x B) pelo algoritmo linha por
 * linha com somas parciais, vetorizado com Intel Intrinsics / AVX.
 *
 * Mesma ordem de loops da versao escalar de somas parciais (i -> k -> j):
 * cada A[i][k] e replicado nas 8 posicoes de um registrador AVX e
 * multiplicado por 8 floats consecutivos da linha k de B, acumulando em 8
 * floats consecutivos da linha i de C com _mm256_mul_ps + _mm256_add_ps.
 *
 * Por que mul + add e nao FMA (_mm256_fmadd_ps): mul e add separados fazem
 * exatamente as mesmas operacoes, na mesma ordem e com os mesmos
 * arredondamentos da versao escalar -- o resultado e identico bit a bit ao
 * da referencia usada por check_mult_errors() em matrix_lib_test.c. O FMA
 * arredonda uma unica vez e, em elementos de C quase nulos por
 * cancelamento, diverge dessa referencia alem da tolerancia relativa de
 * 1e-4 sem que haja erro real. A variante com FMA fica em
 * matrix_avx_fma.c, apenas para comparacao no relatorio.
 *
 * Como B e C sao percorridas por linha (row-major), os loads/stores de 8
 * floats sao contiguos: nenhum gather, nenhum salto entre linhas de cache.
 *
 * Codigos de erro de matrix_matrix_mult (iguais aos das outras versoes):
 *  - (1) colunas de A diferentes das linhas de B
 *  - (2) dimensoes de C incompativeis com A x B
 *  - (3) m1, m2 ou r nulos
 *  - (4) values de m1, m2 ou r nulo
 *
 * scalar_matrix_mult e apenas um stub neste benchmark: o foco e medir
 * matrix_matrix_mult.
 */

#include "matrix_lib.h"
#include <stdio.h>
#include <immintrin.h>

int scalar_matrix_mult(float scalar_value, matrix *m, matrix *r) {
    return 0;
}

/**
 * @brief Calcula r = m1 x m2 por somas parciais em linha, com AVX.
 *
 * Para cada linha i, a linha i de r e zerada e depois, para cada k, recebe
 * m1[i][k] * m2[k][0..q-1] em blocos de 8 elementos (_mm256_mul_ps seguido
 * de _mm256_add_ps).
 *
 * Usa loadu/storeu (sem exigencia de alinhamento): nao ha penalidade com
 * enderecos alinhados a 32 bytes (aligned_alloc(32) do programa de teste)
 * e a funcao continua correta com um malloc comum. Como q e multiplo de 8,
 * o loop vetorial cobre a linha inteira; o loop escalar final so existe
 * por seguranca, para uma matriz fora dessa hipotese.
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

    /* quantidade de colunas coberta pelos blocos de 8 (q inteiro, no trabalho) */
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
