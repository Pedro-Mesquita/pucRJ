#include <stdio.h>

void dump(void *p, int n)
{
    unsigned char *p1 = p;
    while (n--)
    {
        printf("%p - %02x\n", p1, *p1);
        p1++;
    }
}

int main(void)
{
    int i = 10000;
    dump(&i, sizeof(i));
    return 0;
}