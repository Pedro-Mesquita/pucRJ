#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_NOME 81

struct client
{
	char* nome;
	float saldo;
	int idade;
};
typedef struct client Cliente;

struct no {
	Cliente cliente;
	struct no* prox;
};

typedef struct no No;

struct clientAux
{
	char nome[MAX_NOME];
	float saldo;
	int idade;
};

typedef struct client ClientAux;

char mostraEPerguntaOpcao(void);

void listaTodoClientes(No* pNo);
No* incluiUmCliente(No* pNo);
void salvarTodosClientes(No* pNo);




int main(void)
{
	No* pNo;
	pNo = NULL;
	char opcao;

	while ((opcao = mostraEPerguntaOpcao()) != 'z')
	{
		switch (opcao)
		{
		case 'a':
			printf("\n---------- ADICIONAR CLIENTES ----------\n");
			pNo = incluiUmCliente(pNo);
		case 'b':
			break;
		case 'c':
			printf("\n---------- ALTERAR SALDO ----------\n");
			
		case 'd':
			printf("\n---------- EXCLUIR CLIENTES ----------\n");
			
			break;
		case 'e':
			listaTodoClientes(pNo);
		};
	}
	salvarTodosClientes(pNo);

	return 0;
}


void lerArquivos(No* pNo) {
	FILE* arq;
	No* p;
	p = pNo;
	arq = fopen("clientes.bin", "rb");
	if (arq == NULL) {
		fprintf(stderr, "Deu ruim no arquivo\n");
		exit(2);
	}
	//tentar resolver isso
	while (p) {
		fread(p, sizeof(), 1);
	}
}

void salvarTodosClientes(No* pNo) {
	FILE* arq;
	No* p;
	p = pNo;
	arq = fopen("clientes.bin", "wb");
	if (arq == NULL) {
		fprintf(stderr, "Deu ruim no arquivo\n");
		exit(2);
	}
	while (p) {
		fwrite(p->cliente.nome, sizeof(strlen(p->cliente.nome)+1), 1, arq);
		fwrite(&p->cliente.saldo, sizeof(float), 1, arq);
		fwrite(&p->cliente.idade, sizeof(int), 1, arq);
		p = p->prox;
	}
}

void listaTodoClientes(No* pNo) {
	// imprimir na ordem correta com recursão
	No* p;
	p = pNo;
	while(p){
		printf("\nCliente\n");
		printf("Nome: %s\n", p->cliente.nome);
		printf("Saldo: R$%f\n", p->cliente.saldo);
		printf("Idade: %d ano(s)\n", p->cliente.idade);
		p = p->prox;
	}
}

/*
1 - criar uma variável auxiliar
2 - alocar memória
3 - o novo elemento vai apontar para a cabeça da lista
4 - pNo vai começar a apontar o novo elemento da cabeça da lista
*/

No * incluiUmCliente(No* pCabeca) {
	char nomeAux[MAX_NOME];
	No* pAux;
	//criei o no
	pAux = malloc(sizeof(No));
	if (pAux == NULL)
	{
		fprintf(stderr, "Erro ao alocar memória\n");
		exit(123);
	}
	//preenche os dados do cliente
	printf("Entre com o nome do cliente: ");
	scanf("%[^\n]s", nomeAux);
	pAux->cliente.nome = (char*)malloc(strlen(nomeAux) + 1);
	if (!(*pAux).cliente.nome) {
		fprintf(stderr, "Erro ao alocar memória\n");
		exit(33);
	}
	strcpy((*pAux).cliente.nome, nomeAux);
	printf("Entre com o saldo do cliente: ");
	scanf("%f", &pAux->cliente.saldo);
	printf("Entre com a idade: ");
	scanf("%d", &(*pAux).cliente.idade);

	// incluir o no na lista
	pAux->prox = pCabeca;
	pCabeca = pAux;
	return pCabeca;
}


char mostraEPerguntaOpcao(void)
{
	char opcao;
	do
	{
		printf("a) Criar um cliente\n");
		printf("b) Buscar um cliente\n");
		printf("c) Atualizar um cliente\n");
		printf("d) Apagar um cliente\n");
		printf("e) Listar todos os clientes\n");
		printf("z) Terminar\n");
		printf("Insira um valor: ");
		opcao = tolower(getchar());
		while ((getchar() != '\n'))
		{
		}
	} while (!(opcao >= 'a' && opcao <= 'e' || opcao == 'z'));
	return opcao;
}