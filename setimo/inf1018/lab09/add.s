.text
.globl add
add:
    movl %edi, %eax
    addl %esi, %eax
    addl %edx, %eax

    ret
