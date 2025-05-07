.text
.globl add2

add2:
    pushq %rbp
    movq %rsp, %rbp

    cmpq $0, %rdi
    je finaliza
    
    movl 0(%rdi), %eax
    pushq %rax          
    movq 8(%rdi), %rdi
    call add2
    popq %rdi          
    addl %edi, %eax     

finaliza: 
    leave
    ret
            