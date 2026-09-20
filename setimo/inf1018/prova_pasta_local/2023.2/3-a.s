.text
.globl
boo:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp

    movl (%rdi), -4(%rbp) # conteúdo da memória apontado por *a = -4(%rbp)
    movl %esi, -8(%rbpp)  # n = -8(%rbp)
    movl $0, -12(%rbp)    # acc = -12(%rbp)

    l1:
        cmpl $0, -8(%rbp)
        je l2
        movl -4(%rbp), %edi
        movl -8(%rbp), %esi
        
        call corta

        movl -12(%rbp), %r11d
        addl %eax, %r11d
        movl %r11d, -12(%rbp)
        subl $4, -8(%rbp)
        addl $4, -4(%rbp)
        jmp l1
    l2:
        movl -12(%rbp), %eax
        leave
        ret