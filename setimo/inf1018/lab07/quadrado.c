#include <stdio.h>

int main(void)
{
    int valores[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0};
    int *p = valores;

    while (*p)
    {
        printf("%d \n", (*p) * (*p));

        p++;
    }

    return 0;
}