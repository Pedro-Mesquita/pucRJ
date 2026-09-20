.text
.globl boo
boo:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp

    movq %rdi, -8(%rbp)
    movl %esi, -12(%rbp)
    movl %edx, -16(%rbp)

L1:
    cmpl $0, -12(%rbp)
    je L2

    movq -8(%rbp), %rax
    movl (%rax), %edi
    movl -16(%rbp), %esi
    call f

    movq -8(%rbp), %rcx
    movl %eax, 4(%rcx)

    addq $8, -8(%rbp)
    subl $1, -12(%rbp)
    jmp L1

L2:
    leave
    ret