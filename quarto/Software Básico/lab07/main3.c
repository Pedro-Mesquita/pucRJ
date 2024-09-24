#include <stdio.h>

int main(void)
{
    int i = 1;
    while (i < 11)
    {
        printf("%d\n", i * i);
        i++;
    }
    return 0;
}