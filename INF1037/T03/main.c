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
void imprimePacientes(Paciente **ponteiroParaPaciente, int qtdItens);
float calculaModa(Paciente **ponteiroParaPaciente, int qtd);

int main(void)
{
    int qtd;
    qtd = qtdLinhasArquivos("texto.txt");
    Paciente **pPaciente = ponteiroParaPaciente("texto.txt", qtd);
    imprimePacientes(pPaciente, qtd);
    float moda = calculaModa(pPaciente, qtd);
    printf("Última moda encontrada: %.2f\n", moda);
    return 0;
}

void imprimePacientes(Paciente **ponteiroParaPaciente, int qtdItens)
{

    for (int i = 0; i < qtdItens; i++)
    {
        printf("\n%d° cliente:\n", i + 1);
        printf("%s\n", ponteiroParaPaciente[i]->nome);
        printf("%.2f\n", ponteiroParaPaciente[i]->altura);
        printf("%.0f\n", ponteiroParaPaciente[i]->peso);
    }
}

float calculaModa(Paciente **ponteiroParaPaciente, int qtd)
{
    float moda = ponteiroParaPaciente[0]->altura;
    int contagemAtual = 1;
    int maxContagem = 1;

    for (int i = 1; i < qtd; i++)
    {
        if (ponteiroParaPaciente[i]->altura == ponteiroParaPaciente[i - 1]->altura)
        {
            contagemAtual++;
        }
        else
        {
            contagemAtual = 1;
        }
        if (contagemAtual >= maxContagem)
        {
            maxContagem = contagemAtual;
            moda = ponteiroParaPaciente[i]->altura;
        }
    }
    return moda;
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