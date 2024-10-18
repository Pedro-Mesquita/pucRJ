#include <stdio.h>

int acerta(int *u, int i);

int main(void)
{
    int u[] = {1, 2, 3, 4, 5, 6}; // An array of integers
    int retorno = acerta(u, 5);   // Call acerta with the array and index 5
    printf("%d\n", retorno);      // Print the result
    return 0;
}
