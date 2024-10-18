#include <stdio.h>

int corta(int a, int b);

unsigned int inverteOrdemBytes(unsigned int i)
{
    unsigned int byte1 = (i & 0x000000FF) << 24; // Move o 1º byte para a 4ª posição
    unsigned int byte2 = (i & 0x0000FF00) << 8;  // Move o 2º byte para a 3ª posição
    unsigned int byte3 = (i & 0x00FF0000) >> 8;  // Move o 3º byte para a 2ª posição
    unsigned int byte4 = (i & 0xFF000000) >> 24; // Move o 4º byte para a 1ª posição

    return (byte1 | byte2 | byte3 | byte4);
}

int main()
{
    unsigned int valor = 0x01020304;
    unsigned int resultado = inverteOrdemBytes(valor);

    printf("Entrada: 0x%08X\n", valor);
    printf("Resultado: 0x%08X\n", resultado);

    int result = corta(10, 15);

    // Print the result
    printf("= %d\n", result);

    return 0;
}
