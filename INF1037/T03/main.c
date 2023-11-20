
// 33D - Pedro Mesquita Maia - 2312664

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
int buscaBinaria(Paciente **ponteiroParaPaciente, float chave, int tamanhoVetor);
void resultadoBuscaBinaria(int pos, float height, Paciente **pPaciente);
void liberarMemoria(Paciente **ponteiroParaPaciente, int qtd);

int main(void)
{
    int qtd;
    float chave;
    int indice1, indice2, indice3;

    qtd = qtdLinhasArquivos("texto.txt");
    Paciente **pPaciente = ponteiroParaPaciente("texto.txt", qtd);
    imprimePacientes(pPaciente, qtd);
    float moda = calculaModa(pPaciente, qtd);
    printf("Moda: %.2f\n", moda);
    indice1 = buscaBinaria(pPaciente, 1.80, qtd);
    indice2 = buscaBinaria(pPaciente, 1.60, qtd);
    indice3 = buscaBinaria(pPaciente, 2.0, qtd);
    resultadoBuscaBinaria(indice1, 1.80, pPaciente);   
    resultadoBuscaBinaria(indice2, 1.60, pPaciente);   
    resultadoBuscaBinaria(indice3, 2.0, pPaciente);   
    liberarMemoria(pPaciente, qtd);

    return 0;
}


void liberarMemoria(Paciente **ponteiroParaPaciente, int qtd)
{
    for (int i = 0; i < qtd; i++)
    {
        free(ponteiroParaPaciente[i]);
    }
    free(ponteiroParaPaciente);
}


void resultadoBuscaBinaria(int pos, float height, Paciente **pPaciente)
{
    if (pos < 0)
    {
        printf("Não há pacientes com a altura %.2f\n", height);
    }
    else
    {
        printf("Registro de maior peso com altura %.2f: %s - %.2f|%.0f\n", height, pPaciente[pos]->nome, pPaciente[pos]->altura, pPaciente[pos]->peso);
    }
}

int buscaBinaria(Paciente **ponteiroParaPaciente, float chave, int tamanhoVetor)
{
    int inicio;
    int meio;
    int fim;

    inicio = 0;
    fim = tamanhoVetor - 1;

    while (inicio <= fim)
    {
        meio = (inicio + fim) / 2;
        if (chave == ponteiroParaPaciente[meio]->altura)
        {
            for (int i = 0; i < tamanhoVetor; i++)
            {
                if (ponteiroParaPaciente[i]->peso > ponteiroParaPaciente[meio]->peso && ponteiroParaPaciente[i]->altura == ponteiroParaPaciente[meio]->altura)
                {
                    meio = i;
                }
            }
            return meio;
        }
        else if (chave > ponteiroParaPaciente[meio]->altura)
        {
            inicio = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }
    }
    return -1;
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

void imprimePacientes(Paciente **ponteiroParaPaciente, int qtdItens)
{

    for (int i = 0; i < qtdItens; i++)
    {
        printf("%d° cliente: %s - %.2f|%.0f\n", i + 1, ponteiroParaPaciente[i]->nome, ponteiroParaPaciente[i]->altura, ponteiroParaPaciente[i]->peso);
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
        if (fscanf(arq, "%80[^:]: %f %f\n", vetorPacientes[i]->nome, &(vetorPacientes[i]->altura), &(vetorPacientes[i]->peso)) != 3)
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