.text
.globl
diff:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp

    movl $0, %r10d
    cvtsi2sd %r10d, %xmm9
    movsd %xmm9, -8(%rbp)  #s[1]=0.0
    movsd %xmm9, -16(%rbp)  #s[0]=0.0

    l1:
        cmpl $0, %edx
        je l2

        movsd -16(%rbp), %xmm9
        addsd %xmm0, %xmm9
        movsd %xmm9, -16(%rbp)

        movsd -8(%rbp), %xmm9
        addsd %xmm1, %xmm9
        movsd %xmm9, -8(%rbp)
        subl $4, %edx
        jmp l1
    l2:
        movsd -8(%rbp), %xmm9
        movsd -16(%rbp), %xmm0
        subsd %xmm9, %xmm0
        leave
        ret