.text
.globl corta

corta:
    pushq %rbp
    movq %rsp, %rbp

    movl $0, %eax
    addl %edi, %eax
    addl %esi, %eax

    leave
    ret
