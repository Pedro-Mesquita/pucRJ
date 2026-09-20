/**
 * Gera matrizes binarias de floats para os benchmarks, sem depender de
 * Python/numpy (para maquinas como a IAC2, onde nao ha numpy nem pip).
 *
 * Formato: o mesmo esperado por load_matrix() em matrix_lib_test.c --
 * rows * cols floats de 32 bits, row-major, sem cabecalho, endianness nativo.
 *
 * Para cada tamanho gera data/inputs/<R>x<C>/A.dat (R x C) e B.dat (C x R),
 * com valores uniformes em [-1, 1]. A semente e fixa por tamanho, entao os
 * arquivos sao reproduziveis: as mesmas matrizes servem para todas as
 * versoes (requisito para benchmark e corretude justos). Os valores NAO sao
 * os mesmos do generate_matrices.py (gerador diferente); dentro de uma
 * maquina use sempre um so gerador.
 *
 * Compilar:  gcc -O2 -o build/generate_matrices scripts/generate_matrices.c
 * Uso:       build/generate_matrices [-o data/inputs] 8 16 1024 16x32 ...
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/stat.h>

#define SEED 42UL
#define BLOCO_LINHAS 1024

/* xorshift64*: rapido, deterministico, mais que suficiente para dados de teste */
static uint64_t estado;

static void semear(unsigned long rows, unsigned long cols) {
    estado = (rows == cols) ? SEED + rows : SEED + rows * cols + cols;
    estado = estado * 0x9E3779B97F4A7C15ULL + 1;   /* espalha sementes proximas */
}

static float uniforme(void) {
    estado ^= estado >> 12;
    estado ^= estado << 25;
    estado ^= estado >> 27;
    uint64_t x = estado * 0x2545F4914F6CDD1DULL;
    /* 24 bits altos -> [0, 1) com a resolucao de um float; depois [-1, 1) */
    return (float)(x >> 40) / 16777216.0f * 2.0f - 1.0f;
}

static int criar_dir(const char *caminho) {
    /* mkdir -p simplificado: cria cada componente */
    char tmp[1024];
    snprintf(tmp, sizeof tmp, "%s", caminho);
    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(tmp, 0755) && errno != EEXIST) return -1;
            *p = '/';
        }
    }
    return (mkdir(tmp, 0755) && errno != EEXIST) ? -1 : 0;
}

static int gravar(const char *arq, unsigned long rows, unsigned long cols) {
    FILE *f = fopen(arq, "wb");
    if (!f) { perror(arq); return -1; }

    float *bloco = malloc((size_t)BLOCO_LINHAS * cols * sizeof(float));
    if (!bloco) { fprintf(stderr, "sem memoria para o bloco\n"); fclose(f); return -1; }

    for (unsigned long i0 = 0; i0 < rows; i0 += BLOCO_LINHAS) {
        unsigned long linhas = rows - i0 < BLOCO_LINHAS ? rows - i0 : BLOCO_LINHAS;
        size_t n = (size_t)linhas * cols;
        for (size_t k = 0; k < n; k++) bloco[k] = uniforme();
        if (fwrite(bloco, sizeof(float), n, f) != n) {
            perror(arq); free(bloco); fclose(f); return -1;
        }
    }
    free(bloco);
    fclose(f);
    printf("%s: %lux%lu = %lu floats\n", arq, rows, cols, rows * cols);
    return 0;
}

int main(int argc, char *argv[]) {
    const char *base = "data/inputs";
    int i = 1;

    if (argc > 2 && strcmp(argv[1], "-o") == 0) { base = argv[2]; i = 3; }
    if (i >= argc) {
        fprintf(stderr, "uso: %s [-o dir] N | RxC ...   (multiplos de 8)\n", argv[0]);
        return 1;
    }

    for (; i < argc; i++) {
        unsigned long rows, cols;
        char *x = strchr(argv[i], 'x');
        if (x) { *x = '\0'; rows = strtoul(argv[i], NULL, 10); cols = strtoul(x + 1, NULL, 10); }
        else   { rows = cols = strtoul(argv[i], NULL, 10); }

        if (rows == 0 || cols == 0 || rows % 8 || cols % 8) {
            fprintf(stderr, "tamanho invalido: %s (multiplos de 8)\n", argv[i]);
            return 1;
        }

        char dir[512], arq[600];
        snprintf(dir, sizeof dir, "%s/%lux%lu", base, rows, cols);
        if (criar_dir(dir)) { perror(dir); return 1; }

        semear(rows, cols);
        snprintf(arq, sizeof arq, "%s/A.dat", dir);
        if (gravar(arq, rows, cols)) return 1;
        snprintf(arq, sizeof arq, "%s/B.dat", dir);
        if (gravar(arq, cols, rows)) return 1;
    }
    return 0;
}
