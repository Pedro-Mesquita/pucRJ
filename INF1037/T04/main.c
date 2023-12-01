#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


struct no {
    char nome[81];
    char telefone[21];
    struct no* prox;
};

typedef struct no No;

struct agenda {
    No* ponteiroLista[26];
};

typedef struct agenda Agenda;

Agenda* criaAgenda(void);
void insereOrd(Agenda* agenda, No* contato);


int main(void) {
    Agenda* minhaAgenda = criaAgenda();

    free(minhaAgenda);
    return 0;
}



Agenda* criaAgenda() {
    Agenda* agenda = (Agenda*)malloc(sizeof(Agenda));
    if (agenda == NULL) {
        fprintf(stderr, "Erro ao alocar memória\n");
    }
    for (int i = 0; i < 26; i++) {
        agenda->ponteiroLista[i] = NULL;
    }
    return agenda;
}

void insereOrd(Agenda* agenda, No * contato) {
    int pos;
    if (isupper(contato->nome[0])) {
        pos = (contato->nome[0] - 'A');
    }
    else {
        pos = (contato->nome[0] - 'a');
    }

    if (agenda->ponteiroLista[pos] == NULL) {
        agenda->ponteiroLista[pos] = contato;
        return;
    }
    contato->prox;   
}
