.text
.globl foo
foo:
    addl $1, %edi
    movl %edi, %eax

    ret