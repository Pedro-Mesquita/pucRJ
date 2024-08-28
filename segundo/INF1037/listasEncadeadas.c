#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_NOME 81

struct client
{
	char *nome;
	float saldo;
	int idade;
};
typedef struct client Cliente;

struct no
{
	Cliente cliente;
	struct no *prox;
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
void listaTodoClientes(No *pNo);
No *incluiUmCliente(No *pNo);
void salvarTodosClientes(No *pNo);
void liberaClientes(No *pCabeca);
No *apagaCliente(No *pCabeca);
void imprimeInverso(No *pCabeca);

int main(void)
{
	No *pHead;
	pHead = NULL;
	char opcao;

	while ((opcao = mostraEPerguntaOpcao()) != 'z')
	{
		switch (opcao)
		{
		case 'a':
			printf("\n---------- ADICIONAR CLIENTES ----------\n");
			pHead = incluiUmCliente(pHead);
		case 'b':
			break;
		case 'c':
			printf("\n---------- ALTERAR SALDO ----------\n");

		case 'd':
			printf("\n---------- EXCLUIR CLIENTES ----------\n");
			pHead = apagaCliente(pHead);
		case 'e':
			imprimeInverso(pHead);
			printf("\n Lista normal \n");
			listaTodoClientes(pHead);
		};
	}
	salvarTodosClientes(pHead);
	liberaClientes(pHead);

	return 0;
}

No *apagaCliente(No *pCabeca)
{
	No *pAux = pCabeca;
	No *pAnterior;
	pAnterior = NULL; // D
	char nomeAux[MAX_NOME];
	printf("Insira o nome que deseja excluir: ");
	scanf("%[^\n]s", nomeAux);
	// P = D
	if (strcmp(nomeAux, pAux->cliente.nome) == 0)
	{
		free(pAux->cliente.nome);
		pCabeca = pAux->prox;
		free(pAux);
		printf("Chegou aqui");
		return pCabeca;
	}
	// P = C
	while (pAux)
	{
		if (strcmp(nomeAux, pAux->cliente.nome) == 0)
		{
			pAnterior->prox = pAux->prox;
			free(pAux->cliente.nome);
			free(pAux);
			// pAnterior = pAux; // D->PROX = B

			return pCabeca;
		}
		pAnterior = pAux;  // C
		pAux = pAux->prox; // B
	}
	printf("Não foi encontrado nenhum valor");

	return pCabeca;
}

void liberaClientes(No *pCabeca)
{
	No *p;
	No *pAux;

	p = pCabeca;

	while (p)
	{
		free(p->cliente.nome);
		pAux = p->prox;
		free(p);
		p = pAux;
	}
}

void lerArquivos(No *pNo)
{
	FILE *arq;
	No *p;
	p = pNo;
	int tamNome;
	arq = fopen("clientes.bin", "rb");
	if (arq == NULL)
	{
		fprintf(stderr, "Deu ruim no arquivo\n");
		exit(2);
	}
	// tentar resolver isso
	while (fread(&tamNome, sizeof(int), 1, arq) > 0)
	{
		tamNome++;
		p->cliente.nome = (char *)malloc(tamNome * sizeof(char));
		if (p->cliente.nome == NULL)
		{
			fprintf(stderr, "deu ruim!");
			exit(121);
		}

		fread(p->cliente.nome, sizeof(char), 1, arq);
		p->cliente.nome[tamNome] = '\0';
		fread(&p->cliente.saldo, sizeof(float), 1, arq);
		fread(&p->cliente.idade, sizeof(int), 1, arq);
		p->prox = (No *)malloc(sizeof(No));
		p = p->prox;
	}
}

void salvarTodosClientes(No *pNo)
{
	FILE *arq;
	No *p;
	p = pNo;
	arq = fopen("clientes.bin", "wb");
	if (arq == NULL)
	{
		fprintf(stderr, "Deu ruim no arquivo\n");
		exit(2);
	}
	while (p)
	{
		int tamNome = strlen(p->cliente.nome);

		fwrite(&tamNome, sizeof(int), 1, arq);
		fwrite(p->cliente.nome, sizeof(strlen(p->cliente.nome)), 1, arq);
		fwrite(&p->cliente.saldo, sizeof(float), 1, arq);
		fwrite(&p->cliente.idade, sizeof(int), 1, arq);
		p = p->prox;
	}
	fclose(arq);
}

void listaTodoClientes(No *pNo)
{
	// imprimir na ordem correta com recursão
	No *p;
	p = pNo;
	while (p)
	{
		printf("\nCliente\n");
		printf("Nome: %s\n", p->cliente.nome);
		printf("Saldo: R$%.2f\n", p->cliente.saldo);
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

No *incluiUmCliente(No *pCabeca)
{
	char nomeAux[MAX_NOME];
	No *pAux;
	// criei o no
	pAux = malloc(sizeof(No));
	if (pAux == NULL)
	{
		fprintf(stderr, "Erro ao alocar memória\n");
		exit(123);
	}
	// preenche os dados do cliente
	printf("Entre com o nome do cliente: ");
	scanf("%[^\n]s", nomeAux);
	pAux->cliente.nome = (char *)malloc(strlen(nomeAux) + 1);
	if (!(*pAux).cliente.nome)
	{
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

void imprimeInverso(No *pCabeca)
{
	if (pCabeca == NULL)
	{
		return;
	}
	imprimeInverso(pCabeca->prox);
	printf("%s\n", pCabeca->cliente.nome);
}