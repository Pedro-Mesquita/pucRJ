#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_NOME 81
#define MAX_CLIENTES 1000



struct client{
	char nome[MAX_NOME];
	float saldo;
	int idade;
};

typedef struct client Cliente;

int lerClientes(Cliente* pClientes, int max_clientes);
void salvarClientes(Cliente* pClientes, int qtdClientes);
char mostraEPerguntaOpcao(void);
void addCliente(listaClientes, qtdClientes);

int main(void) {
	Cliente listaClientes[MAX_CLIENTES];
	int qtdClientes;
	char opcao;
	// ler todos os clientes do arquivo binário e guardar no vetor listaClientes
	qtdClientes = lerClientes(listaClientes, MAX_CLIENTES);
	// CRUD
	// para implementar o CRUD, criar um loop;
	while ((opcao = mostraEPerguntaOpcao()) != 'z') {
		switch (opcao)
		{
		case 'a':
			// TODO: verificar se ainda pode incluir um cliente
			addCliente(listaClientes, qtdClientes);
			qtdClientes++;
			break;
		case 'b':
			break;
		case 'c':
			break;
		case 'd':
			break;
		case 'e':
			break;
		};
		// depois, salvar os clientes no arquivo
		salvarClientes(listaClientes, qtdClientes);

		return 0;
	}
}

void addCliente(Cliente *pcliente, int qtd) {
	printf("Entre com o nome do cliente: \n");
	scanf("%[^\n]\n", pcliente[qtd].nome);
	printf("Entre com o nome do cliente: \n");
	scanf("%f", &pcliente[qtd].saldo);
	printf("Entre com o nome do cliente: \n");
	scanf("%d", &pcliente[qtd].idade);
}


char mostraEPerguntaOpcao(void){
	char opcao;
	do {
		printf("a) Criar um cliente\n");
		printf("b) Buscar um cliente\n");
		printf("c) Atualizar um cliente\n");
		printf("d) Apagar um cliente\n");
		printf("e) Litar todos os clientes um cliente\n");
		printf("z) Terminar\n");
		printf("Insira um valor: ");
		opcao = tolower(getchar());
		while ((getchar() != '\n')) {}
	} while (!(opcao >= 'a' && opcao <= 'e' || opcao == 'z'));
	return opcao;
}



int lerClientes(Cliente *pClientes, int max_clientes) {
	FILE* arq;
	int qtd;
	arq = fopen("clientes.bin", "rb");
	if (arq == NULL) {
		printf("Deu ruim\n");
		return 0;
	}
	// A função fred vai ler o disco, primeiro ele recebe o endereço onde vai ser salvo -> tamannho do elemento -> a quantidade de elementos a serem lidos 
	// -> e o arquivo onde está salvo as informações
	qtd = fread(pClientes, sizeof(Cliente), max_clientes, arq);
	fclose(arq);

	return qtd;
}

void salvarClientes(Cliente* pClientes, int qtdClientes) {
	FILE* arq;
	int qtdSalva;

	arq = fopen("clientes.bin", "wb");
	if (arq == NULL) {
		fprintf(stderr, "Deu ruim\n");
		exit(2);
	}
	// A função fwrite vai ler o disco, primeiro ele recebe o endereço onde vai ser salvo -> tamannho do elemento -> a quantidade de elementos a serem lidos 
	// -> e o arquivo onde está salvo as informações e retorna quantos ele conseguiu salvar
	qtdClientes =  fwrite(pClientes, sizeof(Cliente), qtdClientes, arq);
	fclose(arq);

	if (qtdClientes != qtdClientes) {
		fprintf(stderr, "Quantidade salva errada.\n");
		exit(-4);
	}
}
