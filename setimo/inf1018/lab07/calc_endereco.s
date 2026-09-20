.data
nums: .byte 65, -105, 111, 34
Sf:   .string "%d\n"

.text
.globl main
main:
  pushq %rbp
  movq  %rsp, %rbp
  subq  $16, %rsp
  movq  %rbx, -8(%rbp)
  movq  %r12, -16(%rbp)

  movq  $nums, %r12
  movl  $0, %eax
  movq  $0, %r10

L1:
  cmpq  $4, %r10
  je    L2

  movq   %r12, %rbx
  addq   %r10, %rbx
  movsbl (%rbx), %edx
  addl   %edx, %eax

  incq   %r10
  jmp    L1

L2:
  movl  %eax, %esi      
  movq  $Sf, %rdi       
  movl  $0, %eax        
  call  printf

  movq  -16(%rbp), %r12
  movq  -8(%rbp), %rbx
  movl  $0, %eax
  leave
  ret