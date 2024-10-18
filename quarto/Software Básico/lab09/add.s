.text
.globl addNum

addNum:
/* Iniciando */
    pushq %rbp
    movq %rsp, %rbp

/* Função */
    movl $0, %eax
    addl %edi, %eax
    addl %esi, %eax
    addl %edx, %eax

/* Finalizando */
    leave
    ret
    