#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_SENHA 10

void criaSenhas(char** vetor, int indice);

int main(void){
    int qtdUsuarios;
    char **aponta_matriz;

    printf("Insidra a quantidade de usuários: ");
    scanf("%d", &qtdUsuarios);

    aponta_matriz = (char **) malloc(qtdUsuarios * sizeof(char*));    
    if(!aponta_matriz){
        exit(1);
    }

    for(int i = 0; i < qtdUsuarios; i++){
        criaSenhas(aponta_matriz, i);
    }

    for(int i = 0; i<qtdUsuarios; i++){
        printf("Usuario #%d, senha: %s\n", i+1, aponta_matriz[i]);
    }

    return 0;
}

void criaSenhas(char** vetor, int indice){
    char senha[TAMANHO_SENHA];
    printf("Entre com a sua senha: \n");
    scanf("%s", senha);

    char* pSenha = malloc((strlen(senha) + 1) * sizeof(char));
    if(!pSenha){
        exit(2);
    }
    strcpy(pSenha, senha);
    vetor[indice] = pSenha;
}