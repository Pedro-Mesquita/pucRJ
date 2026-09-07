/**
 * Programa para multiplicar 1 matriz de doubles por um escalar, colocando o resultado em uma terceira matriz
 * 
 * Código de erro:
 *  - (1) quantidade de linhas diferentes entre m e r
 *  - (2) quantidade de colunas diferentes entre m e r
 *  - (3) m ou r nulos
 *  - (4) values de m ou r nulo
 */

#include "matrix_lib.h"
#include <stdio.h>

int scalar_matrix_mult(float scalar_value, matrix *m, matrix *r) {

    if (m == NULL || r == NULL) {
        fprintf(stderr, "[ERROR %d] Matriz de entrada ou de resposta nula\n", __LINE__);
        return 3;
    }

    if (m->values == NULL || r->values == NULL) {
        fprintf(stderr, "[ERROR %d] Values da matriz de entrada ou de resposta nulo\n", __LINE__);
        return 4;
    }
    if (m->rows!= r->rows) {
        fprintf(stderr, "[ERROR %d] Matriz com quantidade de linhas distintas\n", __LINE__);
        return 1;
    }

    if (m->cols!= r->cols) {
        fprintf(stderr, "[ERROR %d] Matriz com quantidade de colunas distintas\n", __LINE__);
        return 2;
    }

    for (int i = 0; i<m->rows; i++) {
        for (int j = 0; j<m->cols; j++) {
            r->values[i * m->cols + j] = m->values[i*m->cols + j] * scalar_value; 
        }
    }

    return 0;
}

int matrix_matrix_mult(matrix *m1, matrix *m2, matrix *r){
    return 1;
}