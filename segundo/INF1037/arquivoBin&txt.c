#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

FILE* openFile(char* nome, char* modo);

int main(void) {

	FILE* fin;
	FILE* fout;
	char nome[1000];
	int nota;
	fin = openFile("C:/Users/c2312664/Downloads/Nomes.txt", "r");
	fout = openFile("C:/Users/c2312664/Downloads/NomesNotas.txt", "w");

	while (!feof(fin)) {
		fscanf(fin, "%[^\n]\n", nome);
		nota = strlen(nome) % 11;
		fprintf(fout, "%s,  %d\n", nome, nota);
	}

	fclose(fin);
	fclose(fout);

	fin = openFile("C:/Users/c2312664/Downloads/NomesNotas.txt", "r");
	fout = openFile("C:/Users/c2312664/Downloads/NomesNotas.bin", "wb");
	while (!(feof(fin))) {
		fscanf(fin, "%[^,],%d\n", nome, &nota);
		fwrite(nome, 1, 1000, fout); //vetor de caracteres
		//fwrite(nome, 1000, 1, fout); //string
		fwrite(&nota, sizeof nota, 1, fout);
	}

	fclose(fin);
	fclose(fout);
	// lendo bin com feof
	//printf("Lendo arquivo binário: \n");
	//fin = openFile("C:/Users/c2312664/Downloads/NomesNotas.bin", "rb");
	//while (!feof(fin)) {
	//	if (fread(nome, sizeof nome, 1, fin) == 0) break;
	//	fread(&nota, sizeof nota, 1, fin);
	//	printf("%s - %d\n", nome, nota);
	//}

	// lendo bin sem feof
	printf("Lendo arquivo sem feof: \n");
	fin = openFile("C:/Users/c2312664/Downloads/NomesNotas.bin", "rb");
	while (fread(nome, sizeof nome, 1, fin) > 0) {
		fread(&nota, sizeof nota, 1, fin);
		printf("%s - %d\n", nome, nota);
	}
	return 0;
}

FILE* openFile(char* nome, char* modo) {
	FILE* pArq;
	if (!(pArq = fopen(nome, modo))) {
		fprintf(stderr, "deu ruim");
	}
	return pArq;
}