.text
.globl add2
add2:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp

    cmpq $0, %rdi
    je L_null

    movl (%rdi), %eax
    movl %eax, -4(%rbp)

    movq 8(%rdi), %rdi
    call add2

    addl -4(%rbp), %eax
    jmp L_end

L_null:
    movl $0, %eax

L_end:
    leave
    ret