#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
    int dia_entrada;
    int dia_saida;
    printf("Digite o dia de entrada: ");
    scanf("%d", &dia_entrada);
    printf("Digite o dia da saida: ");
    scanf("%d", &dia_saida);
    printf("Voceh ficou %d com o livro\n", dia_saida - dia_entrada);
    return 0;
}
