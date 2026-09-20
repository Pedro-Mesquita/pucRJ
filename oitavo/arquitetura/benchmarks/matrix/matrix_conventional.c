/**
 * Multiplicacao de matrizes de floats (C = A x B) pelo algoritmo
 * CONVENCIONAL linha x coluna, sem SIMD.
 *
 * Cada C[i][j] e o produto escalar da linha i de A pela coluna j de B
 * (ordem i -> j -> k). A leitura de B[k][j] com k variando percorre uma
 * coluna de uma matriz row-major: os elementos consecutivos estao a
 * `cols` floats de distancia, entao cada acesso tende a cair em outra
 * linha de cache. E a versao de referencia para medir o custo dessa falta
 * de localidade em comparacao com as versoes por linha (somas parciais).
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

int scalar_matrix_mult(float scalar_value, matrix *m, matrix *r) {
    return 0;
}

/**
 * @brief Calcula r = m1 x m2 pelo produto linha x coluna convencional.
 *
 * Para cada (i, j), acumula em um float local a soma dos produtos
 * m1[i][k] * m2[k][j] para todo k e grava o total em r[i][j].
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

    for (unsigned long int i = 0; i < n; i++) {
        for (unsigned long int j = 0; j < q; j++) {
            float soma = 0.0f;

            /* linha i de A (sequencial) x coluna j de B (salta q floats) */
            for (unsigned long int k = 0; k < p; k++) {
                soma += m1->values[i * p + k] * m2->values[k * q + j];
            }

            r->values[i * q + j] = soma;
        }
    }

    return 0;
}
