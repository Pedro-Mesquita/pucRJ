#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct no
{
    char nome[81];
    char telefone[21];
    struct no *prox;
};

typedef struct no No;

struct agenda
{
    No *ponteiroLista[26];
};

typedef struct agenda Agenda;

Agenda *criaAgenda(void);
void insereOrd(Agenda *agenda, No *contato);
void listaLetra(Agenda *agenda, char chave_de_pesquisa);

int main(void)
{
    // Agenda *minhaAgenda = criaAgenda();
    // listaLetra(minhaAgenda, 'b');

    // free(minhaAgenda);
    return 0;
}

Agenda *criaAgenda()
{
    Agenda *agenda = (Agenda *)malloc(sizeof(Agenda));
    if (agenda == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória\n");
    }
    for (int i = 0; i < 26; i++)
    {
        agenda->ponteiroLista[i] = NULL;
    }
    return agenda;
}

void insereOrd(Agenda *agenda, No *contato)
{
    int pos;
    No *pAux;
    if (isupper(contato->nome[0]))
    {
        pos = (contato->nome[0] - 'A');
    }
    else
    {
        pos = (contato->nome[0] - 'a');
    }

    if (agenda->ponteiroLista[pos] == NULL)
    {
        agenda->ponteiroLista[pos] = contato;
        return;
    }

    pAux = agenda->ponteiroLista[pos];
    contato->prox = pAux;
    agenda->ponteiroLista[pos] = contato;
}

void listaLetra(Agenda *agenda, char chave_de_pesquisa)
{
    chave_de_pesquisa = toupper(chave_de_pesquisa);
    int pos = chave_de_pesquisa - 'A';
    if (agenda->ponteiroLista[pos] == NULL)
    {
        printf("Não há nenhum registro que comece por '%c'\n", chave_de_pesquisa);
        return;
    }
    No *pNoAux;
    pNoAux = agenda->ponteiroLista[pos];
    printf("%c\n", chave_de_pesquisa);
    while (pNoAux)
    {
        printf("%s: %s\n", agenda->ponteiroLista[pos]->nome, agenda->ponteiroLista[pos]->telefone);
        pNoAux = agenda->ponteiroLista[pos]->prox;
    }
    printf("---------------");
}

int retira(Agenda *agenda, char nome[])
{
    int pos = toupper(nome[0]) - 'A';
    No *pNo = agenda->ponteiroLista[pos];
    No *pAnterior = NULL;

    if (strcmp(pNo->nome, nome) == 0)
    {
        agenda->ponteiroLista[pos] = pNo->prox;
        free(pNo->nome);
        return 1;
    }
    while (pNo)
    {
        if (strcmp(pNo->nome, nome) == 0)
        {
            pAnterior->prox = pNo->prox;
            free(pNo);
            return 1;
        }
        pAnterior = pNo;
        pNo = pNo->prox;
    }
    return 0;
}

void liberaCadastro(Agenda *agenda)
{
    // for (int i = 0; i < 26; i++)
    // {
    //     No *pNo = agenda->ponteiroLista[i];
    //     No *pAux = pNo;
    //     while (pAux)
    //     {
    //         pAux = pNo->prox;
    //         free(pNo);
    //     }
    // }

    free(agenda);
}
