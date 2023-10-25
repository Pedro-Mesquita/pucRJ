
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cypher.h"

FILE *openFile(char *nome, char *modo);

int main(void)
{
    int pos = 0;
    FILE *fin;
    FILE *fout;
    char sentenca[80];
    fin = openFile("./sentencas.txt", "r");
    fout = openFile("./sentencas.bin", "w");

    while (!feof(fin))
    {
        int len = strlen(sentenca);
        fscanf(fin, "%[^,]\n", sentenca);
        cypher(sentenca, KEY, pos, TRUE);
        fprintf(fout, "%d, %s\n", len, sentenca);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

FILE *openFile(char *nome, char *modo)
{
    FILE *pArq;
    if (!(pArq = fopen(nome, modo)))
    {
        fprintf(stderr, "Error\n");
        exit(1);
    }
    return pArq;
}
