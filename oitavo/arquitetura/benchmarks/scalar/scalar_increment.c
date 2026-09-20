/**
 * Multiplicacao de uma matriz de floats por um escalar (r = scalar * m),
 * percorrendo o vetor `values` com ponteiros incrementados sequencialmente.
 *
 * A ordem de acesso a memoria e a mesma da versao linha por linha (o vetor
 * e lido do inicio ao fim), mas sem calcular o indice i * cols + j a cada
 * elemento: os ponteiros de entrada e de saida so avancam uma posicao.
 *
 * Codigos de erro:
 *  - (1) quantidade de linhas diferentes entre m e r
 *  - (2) quantidade de colunas diferentes entre m e r
 *  - (3) m ou r nulos
 *  - (4) values de m ou r nulo
 */

#include "matrix_lib.h"
#include <stdio.h>

/**
 * @brief Calcula r = scalar_value * m avancando ponteiros sobre os vetores.
 *
 * `p_m` e `p_r` comecam no primeiro elemento de m->values e r->values e
 * sao incrementados juntos ate percorrer os rows * cols elementos.
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

    for (unsigned long int k = 0; k < n; k++) {
        *p_r = *p_m * scalar_value;
        p_m++;
        p_r++;
    }

    return 0;
}

int matrix_matrix_mult(matrix *m1, matrix *m2, matrix *r) {
    return 1;
}
