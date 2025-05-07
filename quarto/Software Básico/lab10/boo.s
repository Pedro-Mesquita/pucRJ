.text
.globl boo
boo:
    pushq %rbp
    movq %rsp, %rbp

    subq $24, %rsp
    movq %rbx, -8(%rbp)
    movq %r12, -16(%rbp)
    movq %r11, -24(%rbp)

    movl %esi, %ebx
    movl %edx, %r12d
    movq %rdi, %r11

loop:
    cmpl $0, %ebx
    jle final

    movl 0(%r11), %edi
    movl %r12d, %esi
    call f
    
    movl %eax, 4(%r11)
    addq $8, %r11
    subl $1, %ebx
    
    jmp loop

final:
    movq -8(%rbp), %rbx
    movq -16(%rbp), %r12
    movq -24(%rbp), %r11
    
    leave
    ret
