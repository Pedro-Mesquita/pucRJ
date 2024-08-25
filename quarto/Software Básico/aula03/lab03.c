#include <stdio.h>

int odd_ones(unsigned int x);
unsigned char switch_byte(unsigned char x);

int main(void)
{
    // Parte 1

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

    // Parte 2
    printf("%x tem numero %s de bits\n", 0x01010101, odd_ones(0x01010101) ? "impar" : "par");
    printf("%x tem numero %s de bits\n", 0x01030101, odd_ones(0x01030101) ? "impar" : "par");

    // Parte 3
    unsigned char number = 0xAB;
    unsigned char switchedValue = switch_byte(number);

    return 0;
}

unsigned char switch_byte(unsigned char x)
{
    printf("tamanho: %ld", sizeof(unsigned char));
    return 0;
}

int odd_ones(unsigned int x)
{
    int num_bits = sizeof(x) * 8;
    int i = 0;

    int qtt_one = 0;

    while (i < num_bits)
    {
        if (x & 0x01)
        {
            qtt_one++;
        }

        i++;
        x = x >> 1;
    }

    int rest = qtt_one / 2;

    if (qtt_one != rest * 2)
    {

        return 1;
    }
    else
    {
        return 0;
    }
}
