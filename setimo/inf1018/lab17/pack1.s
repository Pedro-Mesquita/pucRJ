/*struct X1 {
  int vi;
  float vf;
};

double pack1 (struct X1 *px, int n) {
  double acc = 0;
  int i;
  for (i=0;i<n;i++)
    acc += foo(px[i].vf, px[i].vi);
  return acc;
}*/


.text
.globl pack1
pack1:
  pushq %rbp
  movq %rsp, %rbp 
  subq $32, %rsp

  movq %rdi, -8(%rbp)                 # struct X1 *px = -8(%rbp)
  movl %esi, -12(%rbp)                # int n         = -12(%rbp)
  movl $0,   -16(%rbp)                # int i         = -16(%rbp)

  /* Atribuindo zero para acc */
  movq $0, %r10
  cvtsi2sd %r10, %xmm0
  movsd %xmm0, -24(%rbp)            

  L1:
    movl -12(%rbp), %r10d           
    cmp  -16(%rbp), %r10d             
    jle  L2                          

    movq   -8(%rbp),  %rax
    movslq -16(%rbp), %rcx
    imulq  $8, %rcx
    addq   %rcx, %rax                 

    movss  4(%rax), %xmm0             
    cvtss2sd %xmm0, %xmm0            
    movl   0(%rax), %esi              
    call   foo

    movsd -24(%rbp), %xmm1
    addsd %xmm0, %xmm1
    movsd %xmm1, -24(%rbp)

    addl $1, -16(%rbp)                

    jmp L1

  L2:
    movsd -24(%rbp), %xmm0

    leave
    ret



    