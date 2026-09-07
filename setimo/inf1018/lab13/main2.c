#include <stdio.h>
#include <stdint.h>

typedef int (*funcp)(int x);

int add(int x) {
    return x + 1;
}

int main(void) {
    unsigned char codigo[] = {
        0xe9, 0x00, 0x00, 0x00, 0x00,
        0xc3
    };

    int32_t deslocamento = (int32_t)((char *)add - (char *)&codigo[5]);

    codigo[1] = deslocamento & 0xff;
    codigo[2] = (deslocamento >> 8) & 0xff;
    codigo[3] = (deslocamento >> 16) & 0xff;
    codigo[4] = (deslocamento >> 24) & 0xff;

    funcp f = (funcp)codigo;

    int i = f(10);

    printf("valor printado: %d\n", i);

    return 0;
}