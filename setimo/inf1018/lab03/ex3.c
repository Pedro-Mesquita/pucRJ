#include <stdio.h>

unsigned char switch_byte(unsigned char x) {
    unsigned char primeira_parte = x >> 4;
    unsigned char segunda_parte = x << 4;

    unsigned char trocado = primeira_parte | segunda_parte;

    return trocado;
}

unsigned char rotate_left(unsigned char x, int n) {
    unsigned char left = x << n;
    unsigned char wrap = x >> (8 - n);

    return left | wrap;
}

void print_binary(unsigned char x) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (x >> i) & 1);
    }
}

int main() {

unsigned char x = 0x61;
unsigned char r = rotate_left(x, 2);

print_binary(x);
printf(" -> ");
print_binary(r);
printf("\n");
}
