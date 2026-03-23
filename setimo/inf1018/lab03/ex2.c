#include <stdio.h>

int odd_ones(unsigned int x) {
    int parity = 0;

    while (x != 0) {
        int bit = x & 1;

        if (bit == 1) {
            if (parity == 0) {
                parity = 1;
            } else {
                parity = 0;
            }
        }

        x = x >> 1;
    }

    return parity;
}


int main() {
  printf("%x tem numero %s de bits\n",0x01010101,odd_ones(0x01010101) ? "impar":"par");
//   0000 0001 0000 0001 0000 0001 0000 0001
  printf("%x tem numero %s de bits\n",0x01030101,odd_ones(0x01030101) ? "impar":"par");
//   0000 0001 0000 0011 0000 0001 0000 0001
  return 0;
}