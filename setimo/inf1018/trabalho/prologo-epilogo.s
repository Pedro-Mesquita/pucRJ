.text
.globl prologo
prologo:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp

    leave
    ret
