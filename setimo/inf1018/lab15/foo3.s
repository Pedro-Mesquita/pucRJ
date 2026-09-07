/* double foo3 (double *a, int n) {
  int i;
  double r = 0.0;
  for (i=0; i<n; i++) {
    r += sin(*a);
    a++;
  }
  return r;
} */

.text
.globl foo3
foo3: 
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp

    movl $0, -4(%rbp)                # i = -4(%rbp)
    pxor %xmm0, %xmm0
    movsd %xmm0, -12(%rbp)           # r = -12(%rbp)
    movq %rdi, -20(%rbp)             # a = -20(%rbp)
    movl %esi, -24(%rbp)             # n = -24(%rbp) 

L1:
    movl -4(%rbp), %eax
    cmpl -24(%rbp), %eax
    jge L2

    addl $1, -4(%rbp)
    movq -20(%rbp), %rax
    movsd (%rax), %xmm0

    call sin
    addsd -12(%rbp), %xmm0
    movsd %xmm0, -12(%rbp)

    addq $8, -20(%rbp)

    jmp L1

L2:
    movsd -12(%rbp), %xmm0
    leave
    ret 

