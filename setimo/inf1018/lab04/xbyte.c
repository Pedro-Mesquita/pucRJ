#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef unsigned packed_t;

int string2num(char *s, int base)
{
    int a = 0;
    for (; *s; s++)
    {
        if (isdigit(*s))
            a = a * base + (*s - '0');
        else if ((*s >= 'A') && (*s < (base - 10 + 'A')))
            a = a * base + ((*s - 'A') + 10);
        else if ((*s >= 'a') && (*s < (base - 10 + 'a')))
            a = a * base + ((*s - 'a') + 10);
        else
        {
            printf("pane: numero invalido! \n");
            exit(1);
        }
    }
    return a;
}

int xbyte(packed_t word, int bytenum)
{
    return ((int)word << ((3 - bytenum) * 8)) >> 24;
}

int main(int argc, char **argv)
{
    int x;
    if (argc != 3)
    {
        printf("uso: %s <word (em hexadecimal)> <bytenum>\n", argv[0]);
        exit(1);
    }

    x = xbyte(string2num(argv[1], 16), atoi(argv[2]));
    printf("%08x  %d\n", x, x);
    return 0;
}