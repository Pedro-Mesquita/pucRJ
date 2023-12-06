#include <stdlib.h>

struct termo
{
    int coef;
    int grau;
};

typedef struct termo Termo;

int main(void)
{
    return 0;
}

Termo *criaVetorSoma(Termo *primeiroVetor, int tamPrimeiroVetor, Termo *segundoVetor, int tamSegundoVetor)
{
    Termo *resultado = (Termo *)malloc(sizeof(Termo) * (tamPrimeiroVetor + tamSegundoVetor));
    for(int i = 0; i < tamPrimeiroVetor; i++){
        if(primeiroVetor[i].grau == segundoVetor[i].grau){
            resultado[i].coef = primeiroVetor[i].coef + segundoVetor[i].coef;
            resultado[i].grau = primeiroVetor[i].grau;
        }else(
            
        )
    }
}
