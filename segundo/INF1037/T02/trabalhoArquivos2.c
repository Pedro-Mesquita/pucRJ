
#include <stdio.h>
#include <stdlib.h>
#include "cypher.h"

FILE *openFile(char *nome, char *modo);

int main(void)
{
    FILE *fin;
    char sentenca[80];
    int tamanho;
    int pos = 0;

    fin = openFile("./sentencas.bin", "rb");
    while (fread(&tamanho, sizeof tamanho, 1, fin) > 0)
    {
        fread(sentenca, sizeof(char), tamanho, fin);
        cypher(sentenca, KEY, pos, FALSE);
        printf("%s.\n", sentenca);
    }

    fclose(fin);

    return 0;
}

FILE *openFile(char *nome, char *modo)
{
    FILE *pArq;
    if (!(pArq = fopen(nome, modo)))
    {
        fprintf(stderr, "deu erro");
    }
    return pArq;
}
