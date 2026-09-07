.text
.globl foo
foo:
    movl $0, %r10d
    movl $0, %ecx

    L1:
        cmpl %esi, %ecx
        jge L5

        addl (%rdi), %r10d

        cmpl $0, (%rdi)
        jne L4  

        movl %r10d, (%rdi)
        movl $0, %r10d

    L4:
        addq $4, %rdi
        addl $1, %ecx
        jmp L1

    L5:
        ret