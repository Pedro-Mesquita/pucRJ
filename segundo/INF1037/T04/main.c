/*
    33D
    Pedro Mesquita Maia
    2312664
*/

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
int insereOrd(Agenda *agenda, No *contato);
void listaLetra(Agenda *agenda, char chave_de_pesquisa);
int retira(Agenda *agenda, char nome[]);
void liberaCadastro(Agenda *agenda);

int main(void)
{
    Agenda *minhaAgenda = criaAgenda();

    // TESTE

    No *contato1 = (No *)malloc(sizeof(No));
    strcpy(contato1->nome, "Alice um");
    strcpy(contato1->telefone, "123-456-7890");
    contato1->prox = NULL;
    insereOrd(minhaAgenda, contato1);

    No *contato2 = (No *)malloc(sizeof(No));
    strcpy(contato2->nome, "Bob");
    strcpy(contato2->telefone, "987-654-3210");
    contato2->prox = NULL;
    insereOrd(minhaAgenda, contato2);

    No *contato3 = (No *)malloc(sizeof(No));
    strcpy(contato3->nome, "Charlie");
    strcpy(contato3->telefone, "555-123-4567");
    contato3->prox = NULL;
    insereOrd(minhaAgenda, contato3);

    No *contato4 = (No *)malloc(sizeof(No));
    strcpy(contato4->nome, "Alice dois");
    strcpy(contato4->telefone, "testando");
    contato4->prox = NULL;
    insereOrd(minhaAgenda, contato4);

    No *contato5 = (No *)malloc(sizeof(No));
    strcpy(contato5->nome, "Alice tres");
    strcpy(contato5->telefone, "testando");
    contato5->prox = NULL;
    insereOrd(minhaAgenda, contato5);

    No *contato6 = (No *)malloc(sizeof(No));
    strcpy(contato6->nome, "Alice quatro");
    strcpy(contato6->telefone, "testando");
    contato6->prox = NULL;
    insereOrd(minhaAgenda, contato6);

    listaLetra(minhaAgenda, 'A');
    listaLetra(minhaAgenda, 'B');
    listaLetra(minhaAgenda, 'C');
    
    retira(minhaAgenda, "Alice um");
    listaLetra(minhaAgenda, 'A');

    // Liberando a memória alocada
    liberaCadastro(minhaAgenda);

    return 0;
}

Agenda *criaAgenda()
{
    Agenda *agenda = (Agenda *)malloc(sizeof(Agenda));
    if (agenda == NULL)
    {
        exit(1);
    }
    for (int i = 0; i < 26; i++)
    {
        agenda->ponteiroLista[i] = NULL;
    }
    return agenda;
}

int insereOrd(Agenda *agenda, No *contato)
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
        return 1;
    }

    pAux = agenda->ponteiroLista[pos];
    No *pAnterior = NULL;
    while (pAux && strcmp(pAux->nome, contato->nome) < 0)
    {
        pAnterior = pAux;
        pAux = pAux->prox;
    }

    if (pAnterior == NULL)
    {
        contato->prox = agenda->ponteiroLista[pos];
        agenda->ponteiroLista[pos] = contato;
        return 1;
    }
    else
    {
        contato->prox = pAnterior->prox;
        pAnterior->prox = contato;
        return 1;
    }
    return 0;
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
        printf("%s: %s\n", pNoAux->nome, pNoAux->telefone);
        pNoAux = pNoAux->prox;
    }
    printf("---------------\n");
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
    free(agenda);
}
