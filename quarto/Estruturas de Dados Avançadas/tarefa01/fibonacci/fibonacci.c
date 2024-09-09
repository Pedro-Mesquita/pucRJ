#include <stdio.h>

void iterativeFibonacci(int n);
int recursiveFibonacci(int n);
void printFibonacciSequence(int n);

int main(void)
{
    iterativeFibonacci(50);
    printf("Recursivo\n");
    printFibonacciSequence(50);
    return 0;
}

void iterativeFibonacci(int n)
{
    int t1 = 0;
    int t2 = 1;
    int t3;

    printf("%d, %d", t1, t2);

    for (int i = 2; i < n; i++)
    {
        t3 = t1 + t2;
        printf(", %d", t3);
        t1 = t2;
        t2 = t3;
    }
    printf("\n");
}

int recursiveFibonacci(int n)
{
    if (n == 0)
    {
        return 0;
    }
    else if (n == 1)
    {
        return 1;
    }
    else
    {
        return recursiveFibonacci(n - 1) + recursiveFibonacci(n - 2);
    }
}

void printFibonacciSequence(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d, ", recursiveFibonacci(i));
    }
    printf("\n");
}