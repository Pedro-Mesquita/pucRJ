/* Pedro_Mesquita_Maia 2312664 3WA */

#include "cria_func.h"
#include <string.h>

static const unsigned char MOV_RAX_PARA_ARG[3][3] = {
  {0x48, 0x89, 0xc7},   
  {0x48, 0x89, 0xc6},   
  {0x48, 0x89, 0xc2},   
};

static const unsigned char SALVA_ARG_NA_PILHA[3][3] = {
  {0x48, 0x89, 0x7d},   
  {0x48, 0x89, 0x75},   
  {0x48, 0x89, 0x55},   
};


static void emite_b(unsigned char codigo[], int *i, unsigned char b) {
  codigo[(*i)++] = b;
}

static void emite(unsigned char codigo[], int *i, const unsigned char *bytes, int n) {
  for (int k = 0; k < n; k++)
    codigo[(*i)++] = bytes[k];
}

static void emite_imm64(unsigned char codigo[], int *i, unsigned long v) {
  memcpy(&codigo[*i], &v, 8);
  *i += 8;
}

void cria_func(void *f, DescParam params[], int n, unsigned char codigo[]) {
  int i = 0; 

  if (n < 1 || n > 3)
    return;

  int nparam = 0;
  for (int p = 0; p < n; p++)
    if (params[p].orig_val == PARAM)
      nparam++;


  int espaco = ((nparam * 8) + 15) / 16 * 16;

  emite_b(codigo, &i, 0x55);                                  
  emite_b(codigo, &i, 0x48); emite_b(codigo, &i, 0x89);
  emite_b(codigo, &i, 0xe5);                                  
  emite_b(codigo, &i, 0x48); emite_b(codigo, &i, 0x83);
  emite_b(codigo, &i, 0xec);
  emite_b(codigo, &i, (unsigned char) espaco);               

  for (int k = 0; k < nparam; k++) {
    emite(codigo, &i, SALVA_ARG_NA_PILHA[k], 3);
    emite_b(codigo, &i, (unsigned char) (-8 * (k + 1)));  
  }

  int prox_param = 0;  
  for (int p = 0; p < n; p++) {
    switch (params[p].orig_val) {

      case PARAM:
        emite_b(codigo, &i, 0x48); emite_b(codigo, &i, 0x8b);
        emite_b(codigo, &i, 0x45);
        emite_b(codigo, &i, (unsigned char) (-8 * (prox_param + 1)));
        prox_param++;
        break;

      case FIX:
        emite_b(codigo, &i, 0x48); emite_b(codigo, &i, 0xb8);
        if (params[p].tipo_val == INT_PAR)
          emite_imm64(codigo, &i, (unsigned long)(long) params[p].valor.v_int);
        else
          emite_imm64(codigo, &i, (unsigned long) params[p].valor.v_ptr);
        break;

      case IND:
        emite_b(codigo, &i, 0x48); emite_b(codigo, &i, 0xb8);   
        emite_imm64(codigo, &i, (unsigned long) params[p].valor.v_ptr);
        if (params[p].tipo_val == INT_PAR) {
          emite_b(codigo, &i, 0x8b); emite_b(codigo, &i, 0x00); 
        } else {
          emite_b(codigo, &i, 0x48); emite_b(codigo, &i, 0x8b);
          emite_b(codigo, &i, 0x00);                          
        break;
    }

    emite(codigo, &i, MOV_RAX_PARA_ARG[p], 3);
  }

  emite_b(codigo, &i, 0x48); emite_b(codigo, &i, 0xb8);   
  emite_imm64(codigo, &i, (unsigned long) f);
  emite_b(codigo, &i, 0xff); emite_b(codigo, &i, 0xd0); 

  emite_b(codigo, &i, 0xc9); 
  emite_b(codigo, &i, 0xc3); 
}
