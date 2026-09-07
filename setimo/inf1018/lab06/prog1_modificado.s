.data
nums:  .byte 10, -21, -30, 45
Sf:    .string "%d\n"

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

  movl  $0, %ebx        /* ebx = 0; */
  movq  $nums, %r12     /* r12 = &nums */

L1:
  cmpl  $4, %ebx
  je    L2

  movsbl (%r12), %eax 

/*************************************************************/
/* este trecho imprime o valor de %eax (estraga %eax)  */
  movq    $Sf, %rdi     /* primeiro parametro (ponteiro) */
  movl    %eax, %esi    /* segundo parametro (inteiro) */
  call    printf
/*************************************************************/

  addl  $1, %ebx        /* ebx += 1; */
  addq  $1, %r12        /* r12 += 1; */
  jmp   L1

L2:
/***************************************************************/
/* mantenha este trecho aqui e nao mexa - finalizacao!!!!      */
  movq  $0, %rax
  movq  -8(%rbp), %rbx
  movq  -16(%rbp), %r12
  leave
  ret
/***************************************************************/