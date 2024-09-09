#include <stdio.h>

void dump(void *p, int n);
/* programa 1*/
// int main(void)
// {
//     int x = 0xffffffff;
//     int y = 2;
//     printf("x=%d, y=%d\n", x, y);
//     printf("x é menor do que y? %s\n", (x < y) ? "sim" : "nao");
//     return 0;
// }

/* programa 2*/

// int main(void)
// {
//     unsigned int x = 0xffffffff;
//     unsigned int y = 2;
//     printf("x=%u, y=%u\n", x, y);
//     printf("x é menor do que y? %s\n", (x < y) ? "sim" : "nao");
//     return 0;
// }

/* programa 3*/

// int main(void)
// {
//     int x = 0xffffffff;
//     unsigned int y = 2;
//     printf("x=%d, y=%d\n", x, y);
//     printf("x é menor do que y? %s\n", (x < y) ? "sim" : "nao");
//     return 0;
// }

int main(void)
{
    char sc = -1;
    unsigned int ui = sc;

    dump(&ui, sizeof(unsigned int));

    return 0;
}

void dump(void *p, int n)
{
    unsigned char *p1 = p;

    while (n--)
    {
        printf("%p - %02X\n", p1, *p1);
        p1++;
    }
}