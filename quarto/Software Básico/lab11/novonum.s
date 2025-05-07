.data
printText: .string "Número: "
Sf: .string "%d"

.text
.globl novonum

novonum:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp

    movq $printText, %rdi
    call printf

    movq $Sf, %rdi
    leaq -8(%rbp), %rsi
    call scanf

    movl -8(%rbp), %eax
    
    leave
    ret
