#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int buscaBinaria(int* v, int chave, int tamanhoVetor);

int main(void) {
	int v[] = {2,3,8,16,32,64,128,256,512,1024,2027};
	int chave;
	int indice;
	int tamanhoVetor;
	printf("Numero desejado: ");
	scanf("%d", &chave);

	tamanhoVetor = (sizeof v) / (sizeof(int));
	indice = buscaBinaria(v, chave, tamanhoVetor);
	printf("Indice: %d\n", indice);
	// TODO: Continuar aqui...
	return 0;
}

int buscaBinaria(int *v, int chave, int tamanhoVetor) {
	int inicio;
	int meio;
	int fim;

	inicio = 0;
	fim = tamanhoVetor - 1;

	while (inicio <= fim) {
		meio = (inicio + fim) / 2;
		if (chave == v[meio]) {
			return meio;
		}
		else if (chave > v[meio]) {
			inicio = meio + 1;
		}
		else {
			fim = meio - 1;
		}
	}
	return -1;
}
