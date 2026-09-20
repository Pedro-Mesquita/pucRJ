.text
.globl map2
map2:
    pushq %rbp
    movq  %rsp, %rbp
    subq  $32, %rsp

    movq %rdi, -8(%rbp)      # um
    movq %rsi, -16(%rbp)     # outro
    movl %edx, -20(%rbp)     # n
    movl $0, -24(%rbp)       # i = 0

L1:
    movl -24(%rbp), %eax     
    cmpl -20(%rbp), %eax     
    jge L2                   

    movq -8(%rbp), %rax      
    movl -24(%rbp), %ecx     
    movslq %ecx, %rcx        
    leaq (%rax,%rcx,4), %rax 

    movl (%rax), %edi        
    call f                   

    movq -16(%rbp), %rdx     
    movl -24(%rbp), %ecx     
    movslq %ecx, %rcx
    leaq (%rdx,%rcx,4), %rdx 

    movl %eax, (%rdx)        

    addl $1, -24(%rbp)      
    jmp L1

L2:
    leave
    ret