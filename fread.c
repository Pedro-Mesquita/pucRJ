#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char nomeArquivo[] = "C:/Users/c2312664/Desktop/a.dat", dados[100];
	FILE* bFile;

	bFile = fopen(nomeArquivo, "rb");
	if (bFile == NULL) {
		printf("Deu ruim\n");
		exit(1);
	}
	fread(dados, sizeof(char), 100, bFile);

	for (int i = 0; i < 100; i++) {
		printf("%c\n", dados[i]);
	}

	fclose(bFile);
	

	return 0;
}