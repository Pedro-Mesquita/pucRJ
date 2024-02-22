#include <stdio.h>

void operacoes(float *resultado);
void converteHora(int totalSegundos, int *hora, int *min, int *seg);
void valorMaiorqMedia(float valores[], int length);

int main(void)
{
    // float resultado;
    // operacoes(&resultado);
    // printf("%.2f\n", resultado);
    // int hora, min, seg;
    // converteHora(3660, &hora, &min,  &seg);

    float valores[] = {0, 2, 0};
    int len = sizeof(valores) / sizeof(valores[0]);
    valorMaiorqMedia(valores, len);
    return 0;
}

void operacoes(float *resultado)
{
    *resultado = 1 + 1;
}

void converteHora(int totalSegundos, int *hora, int *min, int *seg)
{
    *hora = totalSegundos / 3600;
    *min = totalSegundos - ((*hora) * 3600);
    *seg = ((*min) % 3600);
    printf("%d:%d:%d\n", *hora, *min, *seg);
}

void valorMaiorqMedia(float valores[], int length)
{
    float soma;
    for (int i = 0; i < length; i++)
    {
        soma += valores[i];
    }
    printf("Valor da soma: %.2f\n", soma);
    for (int i = 0; i < length; i++)
    {
        if (valores[i] >= soma)
        {
            printf("%.2f é maior ou igual que a média\n", valores[i]);
            return;
        }
    }
     printf("Só há valores menores que a média\n");
}