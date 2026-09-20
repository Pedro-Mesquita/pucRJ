/**
 * Multiplicacao de matrizes de floats (C = A x B) usando o algoritmo
 * otimizado linha por linha com somas parciais.
 *
 * Em vez de calcular cada C[i][j] como o produto escalar de uma linha de A
 * por uma coluna de B (ordem i -> j -> k), cada elemento A[i][k] e
 * multiplicado por toda a linha k de B e o resultado e acumulado
 * diretamente na linha i de C. Como as matrizes sao
 * row-major, o loop interno percorre B e C sequencialmente na memoria.
 *
 * Codigos de erro de matrix_matrix_mult:
 *  - (1) colunas de A diferentes das linhas de B
 *  - (2) dimensoes de C incompativeis com A x B
 *  - (3) m1, m2 ou r nulos
 *  - (4) values de m1, m2 ou r nulo
 *
 */

#include "matrix_lib.h"
#include <stdio.h>

int scalar_matrix_mult(float scalar_value, matrix *m, matrix *r) {
    return 0;
}

/**
 * @brief Calcula r = m1 x m2 acumulando somas parciais linha a linha.
 *
 * Para cada linha i de m1 e cada elemento m1[i][k], a linha k de m2 e
 * percorrida sequencialmente e m1[i][k] * m2[k][j] e somado em r[i][j].
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

    unsigned long int n = m1->rows;
    unsigned long int p = m1->cols;
    unsigned long int q = m2->cols;

    for (unsigned long int i = 0; i < n; i++) {
        float *c_row = r->values + i * q;

        for (unsigned long int j = 0; j < q; j++) {
            c_row[j] = 0.0f;
        }

        for (unsigned long int k = 0; k < p; k++) {
            float a_ik = m1->values[i * p + k];
            const float *b_row = m2->values + k * q;

            for (unsigned long int j = 0; j < q; j++) {
                c_row[j] += a_ik * b_row[j];
            }
        }
    }

    return 0;
}
