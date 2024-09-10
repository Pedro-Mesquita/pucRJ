#include <stdio.h>
#include <ctype.h>

void dump(void *p, int n);

struct X
{
    int a;
    short b;
    int c;
} x = {0xa1a2a3a4, 0xb1b2, 0xc1c2c3c4};

struct X1
{
    char c1;
    int i;
    char c2;
} x1 = {0xc1, 0x12121212, 0xc2};

struct X2
{
    long l;
    char c;
} x2 = {0xa1a2a3a4a5a6a7a8, 0xc1};

struct X3
{
    int i;
    char c1;
    char c2;
} x3 = {0xa1a2a3a4, 0xc1, 0xc2};

struct X4
{
    struct X2 x;
    char c;
} x4 = {{0xa1a2a3a4a5a6a7a8, 0xc1}, 0xc2};

struct X5
{
    char c1;
    char c2;
    char c3;
} x5 = {0xc1, 0xc2, 0xc3};

struct X6
{
    short s1;
    int i;
    char c[3];
    short s2;
} x6 = {
    0xc1c2,             // 2 bytes + 2 padding
    0xc1c1c3c4,         // 4 bytes
    {0xc1, 0xc2, 0xc3}, // 3 bytes
    0xc1c2};            // 1 padding +  2 bytes + 2 padding

union U1
{
    int i;
    char c[5];
} x7 = {.c = {0xc1, 0xc2, 0xc3, 0xc4, 0xc5}};

union U2
{
    short s;
    char c[5];
} x8 = {.c = {0xc1, 0xc2, 0xc3, 0xc4, 0xc5}};

int main(void)
{

    // printf("x:\n");
    // dump(&x, sizeof(x));
    // printf("tamanho de x %ld\n", sizeof(x));

    // printf("X1:\n");
    // dump(&x1, sizeof(x1));
    // printf("tamanho de x1 %ld\n", sizeof(x1));

    // printf("X2:\n");
    // dump(&x2, sizeof(x2));
    // printf("tamanho de x2 %ld\n", sizeof(x2));

    // printf("X3:\n");
    // dump(&x3, sizeof(x3));
    // printf("tamanho de x3 %ld\n", sizeof(x3));

    // printf("X4:\n");
    // dump(&x4, sizeof(x4));
    // printf("tamanho de x4 %ld\n", sizeof(x4));

    // printf("X5:\n");
    // dump(&x5, sizeof(x5));
    // printf("tamanho de x5 %ld\n", sizeof(x5));

    // printf("X6:\n");
    // dump(&x6, sizeof(x6));
    // printf("tamanho de x6 %ld\n", sizeof(x6));

    // printf("X7:\n");
    // dump(&x7, sizeof(x7));
    // printf("tamanho de x7 %ld\n", sizeof(x7));

    printf("X8:\n");
    dump(&x8, sizeof(x8));
    printf("tamanho de x8 %ld\n", sizeof(x8));

    return 0;
}

void dump(void *p, int n)
{
    unsigned char *pointer = p;

    while (n--)
    {
        printf("%p -  %02X\n", pointer, *pointer);
        pointer++;
    }
}
