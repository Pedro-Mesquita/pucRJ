/**
 * Multiplicacao de uma matriz de floats por um escalar (r = scalar * m),
 * percorrendo a matriz LINHA por linha, com indexacao do vetor.
 *
 * Como a matriz e row-major, o loop interno (colunas) visita floats
 * contiguos na memoria: cada linha de cache carregada e aproveitada por
 * inteiro e o prefetcher reconhece o padrao sequencial.
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
 * @brief Calcula r = scalar_value * m percorrendo m linha por linha.
 *
 * Loop externo nas linhas (i), loop interno nas colunas (j); o elemento
 * (i, j) e acessado por m->values[i * cols + j].
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

    for (unsigned long int i = 0; i < m->rows; i++) {
        for (unsigned long int j = 0; j < m->cols; j++) {
            r->values[i * m->cols + j] = m->values[i * m->cols + j] * scalar_value;
        }
    }

    return 0;
}

int matrix_matrix_mult(matrix *m1, matrix *m2, matrix *r) {
    return 1;
}
