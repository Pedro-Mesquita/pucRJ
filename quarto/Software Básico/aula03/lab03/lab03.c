#include <stdio.h>

int odd_ones(unsigned int x);
unsigned char switch_byte(unsigned char x);
unsigned char rotate_left(unsigned char x, int n);

int main(void)
{
    // PARTE 1

    unsigned int x = 0x87654321; // 1000 0111 0110 0101 0100 0011 0010 0001 - 2271560481
    unsigned int y, z;

    /* o byte menos significativo de x e os outros bits em 0 */
    y = 0x00000001; // 0000 0000 0000 0000 0000 0000 0000 0001 - 1
    y = x & y;

    /* o byte mais significativo com todos os bits em '1'
       e os outros bytes com o mesmo valor dos bytes de x */
    z = 0x77654321; // 0111 0111 0110 0101 0100 0011 0010 0001 - 2003125025
    z = x | z;
    printf("%08x %08x\n", y, z);

    // PARTE 2
    printf("%x tem numero %s de bits\n", 0x01010101, odd_ones(0x01010101) ? "impar" : "par");
    printf("%x tem numero %s de bits\n", 0x01030101, odd_ones(0x01030101) ? "impar" : "par");

    // PARTE 3
    // A)
    unsigned char number = 0xAB;
    unsigned char switchedValue = switch_byte(number);
    printf("%02X\n", switchedValue);

    // B)
    unsigned char k = 0x61;

    printf("%x\n", rotate_left(k, 1)); // Esperado: 0xc2
    printf("%x\n", rotate_left(k, 2)); // Esperado: 0x85
    printf("%x\n", rotate_left(k, 7)); // Esperado: 0xb0

    return 0;
}

unsigned char rotate_left(unsigned char x, int n)
{
    unsigned char circularPart = x >> (8 - n);
    unsigned char shfitedPart = x << n;

    unsigned char result = circularPart | shfitedPart;

    return result;
}

unsigned char switch_byte(unsigned char x)
{
    unsigned char firstHalf = x >> 4;
    unsigned char secondHalf = x << 4;
    unsigned char switchedValue = firstHalf | secondHalf;
    return switchedValue;
}

int odd_ones(unsigned int x)
{
    int num_bits = sizeof(x) * 8;
    int i = 0;
    int qtt_one = 0;

    while (i < num_bits)
    {
        if (x & 1)
        {
            qtt_one++;
        }
        i++;
        x = x >> 1;
    }

    return (qtt_one & 1);
}
