.text
.globl add
add:
    movl $0, %eax
    L1:
        cmpq $0, %rdi 
        je L2

        addl (%rdi), %eax
        movq  8(%rdi), %rdi
        jmp L1
    L2:
        ret