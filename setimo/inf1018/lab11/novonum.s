.data
Sf: .string "numero: "
Sd: .string "%d"

.text
.globl novonum
novonum:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp

    movq $Sf, %rdi
    call printf

    movq $Sd, %rdi
    leaq -4(%rbp), %rsi
    call scanf
    
    movl -4(%rbp), %eax

    leave
    ret 


