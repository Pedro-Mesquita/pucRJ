/*
int nums[] = {10, -21, -30, 45};
int main() {
  int i, *p;
  for (i = 0, p = nums; i != 4; i++, p++)
    if ((*p % 2) == 0)
      printf("%d\n", *p);
  return 0;
}
*/

.data
nums:  .byte  10, -21, -30, 45
Sf:  .string "%d\n"    # string de formato para printf

.text
.globl  main
main:

/********************************************************/
/* mantenha este trecho aqui e nao mexa - prologo !!!   */
  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %rbx, -8(%rbp)
  movq    %r12, -16(%rbp)
/********************************************************/

  movl  $0, %ebx        /* ebx = 0; (i = 0) */
  movq  $nums, %r12     /* r12 = &nums (p = nums) */

L1:
  cmpl  $4, %ebx        /* if (i == 4) ? */
  je    L2              /* vai para L2 if i == 4 */

  movsbl  (%r12), %eax    /* eax = *p */

  testl $1, %eax        /* verifica se é ímpar ou par */
  jnz   L3              /* vai para L3 se o número for ímpar */

  /*************************************************************/
  /* este trecho imprime o valor de %eax (estraga %eax) */
  movq  $Sf, %rdi       /* primeiro parametro (ponteiro) */
  movl  %eax, %esi      /* segundo parametro  (inteiro) */
  call  printf          /* chama a funcao da biblioteca */
  /*************************************************************/

L3:
  addl  $1, %ebx        /* i += 1 */
  addq  $1, %r12        /* p += 1 */
  jmp   L1              /* vai para L1; */

L2:  
/***************************************************************/
/* mantenha este trecho aqui e nao mexa - finalizacao!!!!      */
  movq  $0, %rax        /* rax = 0  (valor de retorno) */
  movq  -8(%rbp), %rbx
  movq  -16(%rbp), %r12
  leave
  ret      
/***************************************************************/
