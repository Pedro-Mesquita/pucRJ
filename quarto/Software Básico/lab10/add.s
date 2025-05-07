.text
.globl add

add:
    pushq %rbp
    movq %rsp, %rbp

    movl $0, %eax     
loop:
    cmpq $0, %rdi     
    je finaliza       
    addl 0(%rdi), %eax  
    movq 8(%rdi), %rdi 
    jmp loop

finaliza:
    leave
    ret
