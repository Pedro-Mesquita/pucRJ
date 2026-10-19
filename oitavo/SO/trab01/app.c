#include "common.h"

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        fprintf(stderr, "uso: %s <nome> <max>\n", argv[0]);
        return 1;
    }

    if (atoi(argv[2]) <= 0)
    {
        fprintf(stderr, "MAX inválido: %s\n", argv[2]);
        return 1;
    }

    char *nome = argv[1];
    int max = atoi(argv[2]);

    setvbuf(stdout, NULL, _IONBF, 0);

    int pc = 0;
    while (pc < max) {
        sleep(1);
        pc++;
        printf("[%s] PC=%d/%d\n", nome, pc, max);
    }

    return 0;
}