#include <stdio.h>
#include "cria_func.h"

/* ===========================================================================
 * Testes da cria_func. Cada teste cobre uma "major" funcionalidade:
 *   1. PARAM            - repassar o argumento recebido
 *   2. FIX              - amarrar uma constante (inclui o exemplo mult x 10)
 *   3. varios PARAM     - mais de um argumento
 *   4. reordenacao      - FIX antes de PARAM (registradores fora de ordem)
 *   5. IND              - ler um valor da memoria na hora da chamada
 *   6. PTR_PAR          - parametro do tipo ponteiro
 * ===========================================================================*/

/* Tipos de ponteiro para a funcao gerada, conforme o numero de argumentos. */
typedef int (*Func0)(void);
typedef int (*Func1)(int);
typedef int (*Func2)(int, int);
typedef int (*FuncP)(char *);

/* --- funcoes "alvo" (f) que serao chamadas pela funcao gerada --- */
int dobro(int x)                 { return 2 * x; }
int soma(int a, int b)           { return a + b; }
int subtrai(int a, int b)        { return a - b; }
int mult(int x, int y)           { return x * y; }
int soma3(int a, int b, int c)   { return a + b + c; }
int primeiro_char(char *s)       { return s[0]; }

/* --- contadores e macro de verificacao --- */
static int total = 0, falhas = 0;

static void verifica(const char *desc, int obtido, int esperado) {
  total++;
  if (obtido == esperado) {
    printf("  ok   | %-28s -> %d\n", desc, obtido);
  } else {
    falhas++;
    printf("  FALHA| %-28s -> obtido %d, esperado %d\n", desc, obtido, esperado);
  }
}

/* 1. PARAM: a funcao gerada apenas repassa seu argumento para dobro(). */
static void teste_param(void) {
  DescParam params[1];
  params[0].tipo_val = INT_PAR;
  params[0].orig_val = PARAM;

  unsigned char codigo[200];
  cria_func(dobro, params, 1, codigo);
  Func1 g = (Func1) codigo;          /* g(x) == dobro(x) == 2x */

  verifica("PARAM dobro(5)",  g(5),  10);
  verifica("PARAM dobro(21)", g(21), 42);
}

/* 2. FIX: amarra a constante 10 no 2o parametro de mult (exemplo do enunciado).
 *    g(x) == mult(x, 10) == 10x */
static void teste_fix(void) {
  DescParam params[2];
  params[0].tipo_val = INT_PAR;  params[0].orig_val = PARAM;
  params[1].tipo_val = INT_PAR;  params[1].orig_val = FIX;  params[1].valor.v_int = 10;

  unsigned char codigo[200];
  cria_func(mult, params, 2, codigo);
  Func1 g = (Func1) codigo;

  verifica("FIX mult(1,10)",  g(1),   10);
  verifica("FIX mult(7,10)",  g(7),   70);
  verifica("FIX mult(10,10)", g(10), 100);
}

/* 3. Varios PARAM: g(a,b) == subtrai(a,b) == a-b */
static void teste_varios_param(void) {
  DescParam params[2];
  params[0].tipo_val = INT_PAR;  params[0].orig_val = PARAM;
  params[1].tipo_val = INT_PAR;  params[1].orig_val = PARAM;

  unsigned char codigo[200];
  cria_func(subtrai, params, 2, codigo);
  Func2 g = (Func2) codigo;

  verifica("2xPARAM subtrai(10,3)", g(10, 3),   7);
  verifica("2xPARAM subtrai(3,10)", g(3, 10),  -7);
}

/* 4. Reordenacao: FIX no 1o parametro, PARAM no 2o.
 *    O unico argumento recebido (em %rdi) precisa ir para o 2o registrador
 *    (%rsi), enquanto o 1o (%rdi) recebe a constante. Testa o salvamento na
 *    pilha. g(x) == subtrai(100, x) */
static void teste_reordenacao(void) {
  DescParam params[2];
  params[0].tipo_val = INT_PAR;  params[0].orig_val = FIX;  params[0].valor.v_int = 100;
  params[1].tipo_val = INT_PAR;  params[1].orig_val = PARAM;

  unsigned char codigo[200];
  cria_func(subtrai, params, 2, codigo);
  Func1 g = (Func1) codigo;

  verifica("REORDEN subtrai(100,30)", g(30), 70);
  verifica("REORDEN subtrai(100,90)", g(90), 10);
}

/* 5. IND: o 2o argumento e lido de uma variavel na memoria, na hora da chamada.
 *    g(x) == soma(x, base). Mudar 'base' entre chamadas muda o resultado. */
static void teste_ind(void) {
  static int base;
  DescParam params[2];
  params[0].tipo_val = INT_PAR;  params[0].orig_val = PARAM;
  params[1].tipo_val = INT_PAR;  params[1].orig_val = IND;  params[1].valor.v_ptr = &base;

  unsigned char codigo[200];
  cria_func(soma, params, 2, codigo);
  Func1 g = (Func1) codigo;

  base = 100;
  verifica("IND soma(5, base=100)", g(5), 105);
  base = 200;                                  /* muda em tempo de execucao */
  verifica("IND soma(5, base=200)", g(5), 205);
}

/* 6. Tres parametros (limite do enunciado), misturando PARAM e FIX.
 *    g(a,b) == soma3(a, b, 7) */
static void teste_tres_param(void) {
  DescParam params[3];
  params[0].tipo_val = INT_PAR;  params[0].orig_val = PARAM;
  params[1].tipo_val = INT_PAR;  params[1].orig_val = PARAM;
  params[2].tipo_val = INT_PAR;  params[2].orig_val = FIX;  params[2].valor.v_int = 7;

  unsigned char codigo[200];
  cria_func(soma3, params, 3, codigo);
  Func2 g = (Func2) codigo;

  verifica("3PARAM soma3(1,2,7)",   g(1, 2),  10);
  verifica("3PARAM soma3(10,20,7)", g(10, 20), 37);
}

/* 7. PTR_PAR: parametro do tipo ponteiro (8 bytes) repassado.
 *    g(s) == primeiro_char(s) */
static void teste_ptr(void) {
  DescParam params[1];
  params[0].tipo_val = PTR_PAR;  params[0].orig_val = PARAM;

  unsigned char codigo[200];
  cria_func(primeiro_char, params, 1, codigo);
  FuncP g = (FuncP) codigo;

  verifica("PTR primeiro_char(\"Hi\")", g("Hi"), 'H');
  verifica("PTR primeiro_char(\"abc\")", g("abc"), 'a');
}

int main(void) {
  printf("== teste_param ==\n");        teste_param();
  printf("== teste_fix ==\n");          teste_fix();
  printf("== teste_varios_param ==\n"); teste_varios_param();
  printf("== teste_reordenacao ==\n");  teste_reordenacao();
  printf("== teste_ind ==\n");          teste_ind();
  printf("== teste_tres_param ==\n");   teste_tres_param();
  printf("== teste_ptr ==\n");          teste_ptr();

  printf("\n----------------------------------------\n");
  if (falhas == 0)
    printf("TODOS OS %d TESTES PASSARAM\n", total);
  else
    printf("%d de %d TESTES FALHARAM\n", falhas, total);

  return falhas != 0;
}
