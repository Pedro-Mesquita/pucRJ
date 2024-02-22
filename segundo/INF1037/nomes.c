#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 45

char** criaListaNomes(int qtd);
void le1Nome(char** pNomes, int posicao);
void imprimeNomes(int qtdNomes, char **nomes);

int main(void) {
    int qtdNomes;
    char** nomes;

    printf("Entre com a quantidade de nomes: ");
    scanf("%d", &qtdNomes);

    nomes = criaListaNomes(qtdNomes);
    for (int i = 0; i < qtdNomes; i++) {
        le1Nome(nomes, i);
    }
    imprimeNomes(qtdNomes, nomes);
    return 0;
}

char** criaListaNomes(int qtd) {
    char** p;
    p = (char**)malloc(qtd * sizeof(char*));
    if (!p) {
        fprintf(stderr, "Erro ao alocar\n");
        exit(1);
    }
    return p;
}

void le1Nome(char** pNomes, int posicao) {
    char nome[TAM_NOME];

    printf("Entre com um nome: ");
    scanf("%s", nome); 
    char* pNome = (char *)malloc((strlen(nome) + 1) * sizeof(char));
    if (!pNome) {
        fprintf(stderr, "Erro ao alocar\n");
        exit(1);
    }
    strcpy(pNome, nome);
    pNomes[posicao] = pNome;
}

void imprimeNomes(int qtdNomes, char **nomes){
    for (int i = 0; i < qtdNomes; i++) {
        printf("Nome %d: %s\n", i + 1, nomes[i]);
        free(nomes[i]); 
    }
    free(nomes); 
}