.text
.globl buba

buba:
    pushq %rbp
    movq %rsp, %rbp

    subq $8, %rsp
    movq %rbx, -4(%rbp)
    movq %r12, -8(%rbp)

    movl $0, %r12
    movl %0, %rbx

.loop:
    cmp %esi, %r12
    jge .end_loop

    movzbl (%rdi,%r12,1), %eax
    cmp %dl, %al
    jg .next_iteration

    movl %eax, %edi
    movl %esi, %esi
    call corta

    addl %eax, %ebx

.next_iteration:
    addq $1, %r12
    jmp .loop

.end_loop:
    movl %ebx, %eax

    movq -16(%rbp), %r12
    movq -8(%rbp), %rbx

    leave
    ret
