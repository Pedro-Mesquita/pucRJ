#include <stdio.h>
#define getsig(x)  ((x)>>31 & 1)
#define getexp(x)  ((x)>>23 & 0xff)
#define getfrac(x) ((x) & 0x7fffff)
#define makefloat(s,e,f) ((s & 1)<<31 | (((e) & 0xff) << 23) | ((f) & 0x7fffff))

typedef union { 
  float f;
  unsigned int i;
 } U;


float float2(float f) {
    U u = {.f = f};
    U result;
    result.i = makefloat(getsig(u.i), getexp(u.i)+1, getfrac(u.i));
    return result.f;
}

int getSign(int i) {
    if (i < 0) {
        return 1;
    }

    return 0;
}

int getExp(int i) {
    if (i == 0) return 0;
    unsigned int abs_val = (i < 0) ? (unsigned int)(-i) : (unsigned int)i;
    int bit_pos = 0;
    while (abs_val > 1) {
        abs_val >>= 1;
        bit_pos++;
    }
    return 127 + bit_pos;
}

int getMantissa(int i) {
    if (i == 0) return 0;
    unsigned int abs_val = (i < 0) ? (unsigned int)(-i) : (unsigned int)i;
    int bit_pos = 0;
    unsigned int temp = abs_val;
    while (temp > 1) {
        temp >>= 1;
        bit_pos++;
    }
    abs_val &= ~(1u << bit_pos);
    return (bit_pos <= 23) ? abs_val << (23 - bit_pos) : abs_val >> (bit_pos - 23);
}

float int2float(int i) {
    if (i == 0) return 0.0f;
    U result;
    result.i = makefloat(getSign(i), getExp(i), getMantissa(i));
    return result.f;
}


int main() {
   int i;

   printf("\n******** int2float ****************\n");
   i = 0;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = 1;  
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = -1;  
   printf(" %d -> %10.4f\n", i, int2float(i));
   i = 0x7fffffff;  
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = -i;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = 12345;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   i = -12345;
   printf(" %d -> %+10.4f\n", i, int2float(i));
   return 0;
}

