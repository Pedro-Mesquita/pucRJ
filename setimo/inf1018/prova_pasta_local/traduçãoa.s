/* void foo (double *vd, int n) {
    while (n--) {
        *vd = 0.0;
        vd++;
    }
}*/

# Demorei 14.53 minutos

.text
.globl foo
foo:
    L1:
        cmpl $0, %esi
        je L2
        movl $0, %esi
        cvtsi2sd %esi, %xmm0
        movsd (%rdi), %xmm1
        movsd %xmm0, %xmm1
        addq $8, %rdi
        subl $1, %esi
        jmp L1
    L2:
        ret

.text
.globl foo
foo:
    L1:
        cmp $0, %esi
        je L2
        movl $0, %r9d
        cvtsi2sd %r9d, %xmm15
        movsd %xmm15, (%rdi)
        addq $8, %rdi
        subl $1, %esi
        jmp L1
    L2:
        ret