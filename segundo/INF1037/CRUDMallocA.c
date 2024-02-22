#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_NOME 81
#define MAX_CLIENTES 1000

struct client
{
	char * nome;
	float saldo;
	int idade;
};

typedef struct client Cliente;

struct clientAux
{
	char nome[MAX_CLIENTES];
	float saldo;
	int idade;
};

typedef struct client ClientAux;

int lerClientes(Cliente* pClientes, int max_clientes);
void salvarClientes(Cliente* pClientes, int qtdClientes);
char mostraEPerguntaOpcao(void);
//void deleteCliente(Cliente* pcliente, int qtd);

void addCliente(Cliente* listaClientes, int qtdClientes);
//void listCliente(Cliente* pcliente, int qtd);
//void listarClienteComID(Cliente* pcliente, int qtd);
//void updateCliente(Cliente* pcliente, int qtd);

int main(void)
{
	Cliente listaClientes[MAX_CLIENTES];
	int qtdClientes;
	char opcao;
	// ler todos os clientes do arquivo binário e guardar no vetor listaClientes
	qtdClientes = lerClientes(listaClientes, MAX_CLIENTES);
	// CRUD
	// para implementar o CRUD, criar um loop;
	while ((opcao = mostraEPerguntaOpcao()) != 'z')
	{
		switch (opcao)
		{
		case 'a':
			// TODO: verificar se ainda pode incluir um cliente
			printf("\n---------- ADICIONAR CLIENTES ----------\n");
			incluiUmCliente(listaClientes, qtdClientes);
			qtdClientes++;
			break;
		case 'b':
			break;
		case 'c':
			printf("\n---------- ALTERAR SALDO ----------\n");
			//listarClienteComID(listaClientes, qtdClientes);
			//updateCliente(listaClientes, qtdClientes);
			//break;
		case 'd':
			printf("\n---------- EXCLUIR CLIENTES ----------\n");
			//listarClienteComID(listaClientes, qtdClientes);
			//deleteCliente(listaClientes, qtdClientes);
			//qtdClientes--;
			break;
		case 'e':
			//listCliente(listaClientes, qtdClientes);
			//break;
		};
		// depois, salvar os clientes no arquivo
		salvarClientes(listaClientes, qtdClientes);

		return 0;
	}
}


void incluiUmCliente(Cliente * pcliente, int qtd) {
	char nomeAux[MAX_NOME];
	printf("Entre com o nome do cliente: ");
	scanf("%[^\n]s", nomeAux);
	pcliente[qtd].nome = (char *)malloc(strlen(nomeAux)+1); // +1 por conta do /0
	if (!pcliente[qtd].nome) {
		fprintf(stderr, "erro ao alocar espalo para o nome do cliente \n");
		exit(223);
	}
	strcpy(pcliente[qtd].nome, nomeAux);
	printf("Entre com o saldo do cliente: ");
	scanf("%f", &pcliente[qtd].saldo);
	printf("Entre com a idade do cliente: ");
	scanf("%d", &pcliente[qtd].idade);
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

int lerClientes_A(Cliente* pClientes, int max_clientes)
{
	FILE* arq;
	int qtd = 0;
	int tamNome;
	arq = fopen("clientes.bin", "rb");
	if (arq == NULL)
	{
		printf("Deu ruim\n");
	}
	// A função fred vai ler o disco, primeiro ele recebe o endereço onde vai ser salvo -> tamannho do elemento -> a quantidade de elementos a serem lidos
	// -> e o arquivo onde está salvo as informações
	
	while (fread(&tamNome, sizeof tamNome, 1, arq) > 0); {
		pClientes[qtd].nome = (char *)malloc(tamNome);
		if (!(pClientes[qtd].nome) == NULL) {
			fprintf(stderr, "deu ruim!");
			exit(121);
		}
		fread(pClientes[qtd].nome, tamNome, 1, arq);
		fread(&pClientes[qtd].saldo, sizeof(float), 1, arq);
		fread(&pClientes[qtd].idade, sizeof(int), 1, arq);
		qtd++;
	}
	fclose(arq);
	return qtd;
}


int lerClientes_B(Cliente* pClientes, int max_clientes)
{
	FILE* arq;
	int qtd = 0;
	clientAux cliente;
	arq = fopen("clientes.bin", "rb");
	if (arq == NULL)
	{
		printf("Deu ruim\n");
	}
	// A função fred vai ler o disco, primeiro ele recebe o endereço onde vai ser salvo -> tamannho do elemento -> a quantidade de elementos a serem lidos
	// -> e o arquivo onde está salvo as informações
	
	while (!fread(&cliente, sizeof(ClientAux), 1, arq)) {

		pClientes[qtd].nome = (char*)malloc(strlen(cliente.nome)+1);

		strcpy(pClientes[qtd].nome, cliente.nome);
		pClientes[qtd].saldo = cliente.saldo;
		pClientes[qtd].idade = cliente.idade;
		qtd++;
	}

	fclose(arq);
	return qtd;
}



void salvarClientes_A(Cliente* pClientes, int qtdClientes)
{
	FILE* arq;
	int tamNome;
	arq = fopen("clientes.bin", "wb");
	if (arq == NULL)
	{
		fprintf(stderr, "Deu ruim\n");
		exit(2);
	}
	// A função fwrite vai ler o disco, primeiro ele recebe o endereço onde vai ser salvo -> tamannho do elemento -> a quantidade de elementos a serem lidos
	// -> e o arquivo onde está salvo as informações e retorna quantos ele conseguiu salvar

	for (int i = 0; i < qtdClientes; i++) {
		tamNome = strlen(pClientes[i].nome + 1);
		fwrite(&tamNome, sizeof tamNome, 1, arq);
		fwrite(pClientes[i].nome, tamNome, 1, arq);
		fwrite(&pClientes[i].saldo, sizeof(float), 1, arq);
		fwrite(&pClientes[i].idade, sizeof(int), 1, arq);
	}
	fclose(arq);
}

void salvarClientes_B(Cliente* pClientes, int qtdClientes) 
{
	FILE* arq; 
	ClientAux cliente;
	arq = fopen("clientes.bin", "wb");
	if (arq == NULL)
	{
		fprintf(stderr, "Deu ruim\n");
		exit(2);
	}
	for (int i = 0; i < qtdClientes; i++) {
		{
			cliente.idade = pClientes[i].idade;
			cliente.saldo = pClientes[i].saldo;
			strcpy(cliente.nome, pClientes[i].nome);
			fwrite(&cliente, sizeof cliente, 1, arq);
		}

	fclose(arq);
	}
}