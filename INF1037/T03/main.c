#include <stdio.h>
#include <stdlib.h>

struct paciente
{
    char nome[81];
    float altura;
    float peso;
};

typedef struct paciente Paciente;

int qtdLinhasArquivos(char *nomeDoArquivo);
Paciente **ponteiroParaPaciente(char *nomeDoArquivo, int qtdLinhas);
void imprimeItemDois(Paciente **ponteiroParaPaciente);

int main(void)
{
    int qtd;
    qtd = qtdLinhasArquivos("texto.txt");
    Paciente **pPaciente = ponteiroParaPaciente("texto.txt", qtd);
    imprimeItemDois(pPaciente);
    return 0;
}

void imprimeItemDois(Paciente **ponteiroParaPaciente)
{
    if (ponteiroParaPaciente[2] != NULL)
    {
        printf("Valores do terceiro item:\n");
        printf("%s\n", ponteiroParaPaciente[2]->nome);
        printf("%.2f\n", ponteiroParaPaciente[2]->altura);
        printf("%.0f\n", ponteiroParaPaciente[2]->peso);
    }
    else
    {
        fprintf(stderr, "deu ruim chefe.\n");
    }
}

Paciente **ponteiroParaPaciente(char *nomeDoArquivo, int qtdLinhas)
{
    char nome[81];
    float peso;
    float altura;
    FILE *arq;
    Paciente **vetorPacientes = (Paciente **)malloc(qtdLinhas * sizeof(Paciente *));

    if (vetorPacientes == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória ao vetor de ponteiros para Pacientes\n");
        exit(121);
    }
    arq = fopen(nomeDoArquivo, "r");
    if (arq == NULL)
    {
        fprintf(stderr, "Não foi possível abrir o arquivo '%s'\n", nomeDoArquivo);
        exit(123);
    }

    for (int i = 0; i < qtdLinhas; i++)
    {
        vetorPacientes[i] = (Paciente *)malloc(sizeof(Paciente));
        if (vetorPacientes[i] == NULL)
        {
            fprintf(stderr, "Erro ao alocar memória\n");
            exit(121);
        }
        if (fscanf(arq, "%80[^:]: %f %f", vetorPacientes[i]->nome, &(vetorPacientes[i]->altura), &(vetorPacientes[i]->peso)) != 3)
        {
            fprintf(stderr, "Erro ao ler os dados do paciente na linha %d\n", i + 1);
            exit(122);
        }
    }

    fclose(arq);
    return vetorPacientes;
}

int qtdLinhasArquivos(char *nomeDoArquivo)
{
    char nome[81];
    float peso;
    float altura;
    FILE *arq;
    arq = fopen(nomeDoArquivo, "r");

    if (arq == NULL)
    {
        fprintf(stderr, "Não foi possível abrir o arquivo '%s'\n", nomeDoArquivo);
        return -1;
    }

    int qtd = 0;

    while (fscanf(arq, "%80[^:]: %f %f", nome, &peso, &altura) == 3)
    {
        qtd++;
    }

    fclose(arq);
    return qtd;
}