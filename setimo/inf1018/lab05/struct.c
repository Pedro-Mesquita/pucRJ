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
struct X
{
    int a;
    short b;
    int c;
} x = {0xa1a2a3a4, 0xb1b2, 0xc1c2c3c4};

int main(void)
{
    printf("x: \n");
    dump(&x, sizeof(x));
    printf("\n");
    printf("sizeof(x): %ld\n", sizeof(x));

    return 0;
}
