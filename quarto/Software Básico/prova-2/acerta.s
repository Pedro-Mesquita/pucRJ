.text
.globl acerta

acerta:
    pushq %rbp
    movq %rsp, %rbp

    imull $4, %esi
    movq %rdi, %rax
    addq %rsi, %rax
    movl (%rax), %eax

    leave
    ret


boo: 
    pushq %rbp
    movq %rsp, %rbp

    subq $4, %rsp
    movq %r12, -4(%rbp)


    movq $0, %r12


    compq %


    movq -4(%rbp), %r12
    leave
    ret

