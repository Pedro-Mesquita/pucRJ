.text
.globl foo

foo:
    movq %rdi, %rax
    addq $1, %rax

    ret


/*
01001000 10001001 11111000
01001000 10000011 11000000 00000001
11000011
*/
