#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX_NOME 110

int main(void) {
	FILE* fin;
	FILE* fout;
	char nome[TAM_MAX_NOME];
	int nota;
	fin = fopen("C:/Users/c2312664/Downloads/Nomes.txt", "r");
	fout = fopen("C:/Users/c2312664/Downloads/NomesNotas.txt", "w");
	if (fin == NULL) {
		fprintf(stderr, "Deu ruim\n");
		exit(1);
	}
	if (fout == NULL) {
		fprintf(stderr, "Deu ruim\n");
		exit(2);
	}

	while (fscanf(fin, "%[^\n]\n", nome) == 1) {
		nota = strlen(nome) % 11;
		printf("nome e nota: %s - %d\n", nome, nota);
		fprintf(fout, " %s,%d\n", nome, nota);
	}
	fclose(fin);
	fclose(fout);

	if (!(fin = fopen("C:/Users/c2312664/Downloads/NomesNotas.txt", "r"))) {
		fprintf(stderr, "Deu ruim\n");
		exit(3);
	}
	int notaCampeao = -1;

	while (fscanf(fin, "%[^,],%d\n", nome, &nota) > 0) {
		printf("nome e nota: %s - %d\n", nome, nota);
	}


	return 0;
}
