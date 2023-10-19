#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int numberOfBlanks(char* s);
void copiaStrRec(char* sOrigem, char* sDestino);

int main(void) {
    char texto[] = "TOPZEIRA EIRA mamaco top top top";

    int n;
    n = numberOfBlanks(texto);
    printf("valor calculado: %d", n);
    copiaStrRec(texto, "um outro texto topzeira");
    return 0;
}

int numberOfBlanks(char *s) {
    int acc;
    if (*s == '\0') {
        acc = 0;
    }
    else {
        if (*s == ' ') {
            acc = 1;
        }
        else {
            acc = 0;
        }acc += numberOfBlanks(s+1);
    }
    return acc;
}

void copiaStrRec(char* sOrigem, char* sDestino) {
    if (*sOrigem == '\0') {
        *sDestino = '\0';
    }
    else {
        *sDestino = *sOrigem;
        copiaStrRec(sDestino + 1, sOrigem + 1);
    }
}

// fazer a função como loop
