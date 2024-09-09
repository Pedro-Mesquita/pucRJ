#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef unsigned packed_t;
int xbyte(packed_t word, int bytenum);

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
    int byte = (word >> (bytenum * 8)) & 0xFF;

    // char c;
    //  int i
    // i = c;

    return (byte << 24) >> 24;
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

/*
ana@sol:~/sb$ ./xbyte 01 ab cd 02 0
00000002  2
ana@sol:~/sb$ ./xbyte 11 a0 32 b5 0
ffffffb5  -75
ana@sol:~/sb$ ./xbyte 11 a0 32 b5 1
00000032  50
ana@sol:~/sb$ ./xbyte 11 a0 32 b5 2
ffffffa0  -96
ana@sol:~/sb$ ./xbyte 11 a0 32 b5 3
00000011  17
ana@sol:~/sb$ ./xbyte ab cd 3
00000000  0
ana@sol:~/sb$ ./xbyte ab cd 1
ffffffab  -85
ana@sol:~/sb$ ./xbyte zz zz 1
pane: numero invalido!
*/