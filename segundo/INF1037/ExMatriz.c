#include <stdio.h>

#define COL 5
#define LIN 4

float media(float despesas[][COL], int linha, int coluna, int tipoDespesa, int colunaInicial, int colunaFinal);
float tot_semana(float despesas[][COL], int linhas, int colunas, int semana);

int main(void){

    float despesas[LIN][COL] = {
        {1.1, 2.2, 3.3, 4.4, 5.5},
        {6.6, 7.7, 8.8, 9.9, 10.0},
        {11.1, 12.2, 13.3, 14.4, 15.5},
        {16.6, 17.7, 18.8, 19.9, 20.0},
    };

    float calculaMedia = media(despesas, LIN, COL, 2, 1, 3);
    printf("Media: %f\n", calculaMedia);
    float calculaSemana = tot_semana(despesas, LIN, COL, 0);
    printf("Soma semana: %f\n", calculaSemana);
    return 1;
}


float media(float despesas[][COL], int linha, int coluna, int tipoDespesa, int colunaInicial, int colunaFinal){
    int acc = 0;
    while (colunaInicial <= colunaFinal) {
        acc = acc + despesas[tipoDespesa][colunaInicial];
        colunaInicial++;
    }
    return acc;
}

float tot_semana(float despesas[][COL], int linhas, int colunas, int semana){
    int acc = 0;
    for(int i = 0; i < linhas; i++){
        acc = acc + despesas[i][semana];
    }
    return acc;
}