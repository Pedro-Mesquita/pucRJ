.data
nums: .int 3, -5, 7, 8, -2
s1:   .string "%d\n"

.text
.globl main
main: 
/* prologo */
  pushq %rbp
  movq  %rsp, %rbp
  subq  $16, %rsp
  movq  %rbx, -8(%rbp)
  movq  %r12, -16(%rbp)

/* coloque seu codigo aqui */
  movq $nums, %r12
  movq $0, %rbx

L1:
  cmpq $5, %rbx
  je L2

  movl (%r12), %edi
  movl $1, %esi
  call filtro

  movq $s1, %rdi
  movl %eax, %esi
  call printf

  addq $1, %rbx
  addq $4, %r12

  jmp L1

L2:
/* finalizacao */
  movq -8(%rbp), %rbx
  movq -16(%rbp), %r12
  leave
  ret