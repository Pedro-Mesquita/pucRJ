#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_NOME 81
#define MAX_CLIENTES 1000

struct client
{
	char nome[MAX_NOME];
	float saldo;
	int idade;
};

typedef struct client Cliente;

int lerClientes(Cliente *pClientes, int max_clientes);
void salvarClientes(Cliente *pClientes, int qtdClientes);
char mostraEPerguntaOpcao(void);
void deleteCliente(Cliente *pcliente, int qtd);
void addCliente(Cliente *listaClientes, int qtdClientes);
void listCliente(Cliente *pcliente, int qtd);
void listarClienteComID(Cliente *pcliente, int qtd);
void updateCliente(Cliente *pcliente, int qtd);

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
			addCliente(listaClientes, qtdClientes);
			qtdClientes++;
			break;
		case 'b':
			break;
		case 'c':
			printf("\n---------- ALTERAR SALDO ----------\n");
			listarClienteComID(listaClientes, qtdClientes);
			updateCliente(listaClientes, qtdClientes);
			break;
		case 'd':
			printf("\n---------- EXCLUIR CLIENTES ----------\n");
			listarClienteComID(listaClientes, qtdClientes);
			deleteCliente(listaClientes, qtdClientes);
			qtdClientes--;
			break;
		case 'e':
			listCliente(listaClientes, qtdClientes);
			break;
		};
		// depois, salvar os clientes no arquivo
		salvarClientes(listaClientes, qtdClientes);

		return 0;
	}
}

void searchCliente(Cliente *pcliente, int qtd)
{
	int id;
	printf("1)nome\n");
	printf("1)saldo\n");
	printf("1)idade\n");

	scanf("%d", &id);
	if (id = 1)
	{
		printf("Insira o nome a ser buscado: ");
		char nomeBuscado[81];
		scanf("%[^\n]s", nomeBuscado);
		for (int i = 0; i < qtd; i++)
		{
			if(tolower(pcliente[i].nome) == tolower(nomeBuscado)){
				printf("%s - %.2f - %d\n", pcliente[i].nome, pcliente[i].saldo, pcliente[i].idade);
			}
		}
	}
	if (id = 2)
	{
		printf("Insira o saldo a ser buscado: ");
	}
	if (id = 3)
	{
		printf("Insira a idade a ser buscada: ");
	}
}

void updateCliente(Cliente *pcliente, int qtd)
{
	int id;
	float novoSaldo;
	printf("Insira o id do cliente que deseja alterar: ");
	scanf("%d", &id);
	printf("Insira o novo saldo: ");
	scanf("%f", &novoSaldo);
	pcliente[id - 1].saldo = novoSaldo;
	printf("Saldo alterado com sucesso!\n");
}

void deleteCliente(Cliente *pcliente, int qtd)
{
	int id;
	printf("Insira o id do cliente que deseja excluir: ");
	scanf("%d", &id);
	if (id >= 1 && id <= qtd)
	{
		for (int i = id; i < (qtd); i++)
		{
			pcliente[i - 1] = pcliente[i];
		}
	}
}

void addCliente(Cliente *pcliente, int qtd)
{
	printf("Entre com o nome do cliente: ");
	scanf("%[^\n]s", pcliente[qtd].nome);
	printf("Entre com o saldo do cliente: ");
	scanf("%f", &pcliente[qtd].saldo);
	printf("Entre com a idade do cliente: ");
	scanf("%d", &pcliente[qtd].idade);
}

void listCliente(Cliente *pcliente, int qtd)
{
	printf("\n---------- LISTA DE CLIENTES ----------\n");
	for (int i = 0; i < qtd; i++)
	{
		printf("Nome: %s\nSaldo: %.2f\nIdade: %d\n", pcliente[i].nome, pcliente[i].saldo, pcliente[i].idade);
		printf("----------\n");
	}
}

void listarClienteComID(Cliente *pcliente, int qtd)
{
	for (int i = 0; i < qtd; i++)
	{
		printf("%d) %s\n", i + 1, pcliente[i].nome);
	}
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

int lerClientes(Cliente *pClientes, int max_clientes)
{
	FILE *arq;
	int qtd;
	arq = fopen("clientes.bin", "rb");
	if (arq == NULL)
	{
		printf("Deu ruim\n");
		return 0;
	}
	// A função fred vai ler o disco, primeiro ele recebe o endereço onde vai ser salvo -> tamannho do elemento -> a quantidade de elementos a serem lidos
	// -> e o arquivo onde está salvo as informações
	qtd = fread(pClientes, sizeof(Cliente), max_clientes, arq);
	fclose(arq);

	return qtd;
}

void salvarClientes(Cliente *pClientes, int qtdClientes)
{
	FILE *arq;
	int qtdSalva;

	arq = fopen("clientes.bin", "wb");
	if (arq == NULL)
	{
		fprintf(stderr, "Deu ruim\n");
		exit(2);
	}
	// A função fwrite vai ler o disco, primeiro ele recebe o endereço onde vai ser salvo -> tamannho do elemento -> a quantidade de elementos a serem lidos
	// -> e o arquivo onde está salvo as informações e retorna quantos ele conseguiu salvar
	qtdClientes = fwrite(pClientes, sizeof(Cliente), qtdClientes, arq);
	fclose(arq);

	if (qtdClientes != qtdClientes)
	{
		fprintf(stderr, "Quantidade salva errada.\n");
		exit(-4);
	}
}
