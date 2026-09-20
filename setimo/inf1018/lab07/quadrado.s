.data
valores: .byte 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0
Sf:  .string "%d\n"

.text
.globl main
.extern printf

main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    movq %rbx, -8(%rbp)
    movq %r12, -16(%rbp)

    movq $valores, %r12

L1:
    cmpb $0, (%r12)
    je L2

    movsbl (%r12), %eax
    imull %eax, %eax

    movq $Sf, %rdi
    movl %eax, %esi
    movl $0, %eax
    call printf

    addq $1, %r12
    jmp L1

L2:
    movq $0, %rax
    movq -16(%rbp), %r12
    movq -8(%rbp), %rbx
    leave
    ret