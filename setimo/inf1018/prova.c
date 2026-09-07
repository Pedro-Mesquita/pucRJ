#include <stdio.h>

unsigned char configuraBits(unsigned char byteConf, int bitInicial, int bitFinal) {
    int i = 0;
    while (i <= 7) {
        if (i>=bitInicial && i<=bitFinal) {
            byteConf = byteConf | (1 << i);
        }

        i++;
    }

    return byteConf;
}

// bitConf = 1000 0000
// inicial = 1
// final = 3

// 1000 1110

int main (void) {
    printf("0x%x \n", configuraBits(0x80, 1, 3));
    return 0;
}