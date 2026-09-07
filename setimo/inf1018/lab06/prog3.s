/*
int nums[] = {10, -12, -30, 45};
int main() {
  int i, *p;
  for (i = 0, p = nums; i != 4; i++, p++)
    if ((*p % 2) == 0)
      printf("%d\n", *p);
  return 0;
}
*/

.data
nums: .int 10, -12, -30, 45
Sf: .string "%d\n"

.text
.globl main
main:
/********************************************************/
/* mantenha este trecho aqui e nao mexa - prologo !!!   */
  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %rbx, -8(%rbp)
  movq    %r12, -16(%rbp)
/********************************************************/

  movl $0, %ebx
  movq $nums, %r12

L1:
  cmpl $4, %ebx
  je L2

  movl (%r12), %eax
  movl %eax, %r8d
  andl $1, %r8d
  cmpl $0, %r8d
  jne L3

/*************************************************************/
/* este trecho imprime o valor de %eax (estraga %eax)  */
  movq    $Sf, %rdi    /* primeiro parametro (ponteiro)*/
  movl    %eax, %esi   /* segundo parametro  (inteiro) */
  call    printf       /* chama a funcao da biblioteca */
/*************************************************************/

L3:
  addl  $1, %ebx
  addq  $4, %r12
  jmp L1

L2:
/***************************************************************/
/* mantenha este trecho aqui e nao mexa - finalizacao!!!!      */
  movq  $0, %rax  /* rax = 0  (valor de retorno) */
  movq  -8(%rbp), %rbx
  movq  -16(%rbp), %r12
  leave
  ret
/***************************************************************/