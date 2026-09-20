.text
.globl fat
fat:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp

    movl %edi, -4(%rbp)

    cmpl $0, %edi
    jne L2

    movl $1, %eax
    jmp L3

L2:
    subl $1, %edi
    call fat
    imull -4(%rbp), %eax

L3:
    leave
    ret