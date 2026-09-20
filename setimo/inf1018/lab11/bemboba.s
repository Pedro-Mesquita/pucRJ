.text
.globl bemboba
bemboba:
    pushq %rbp
    movq %rsp, %rbp
    subq $48, %rsp

    movl %edi, -36(%rbp)        /* salva num */

    movl $0, -4(%rbp)           /* int i = 0 */
    leaq -32(%rbp), %rax        /* endereço de local[0] */
    movq %rax, -16(%rbp)        /* a = local */

L1:
    cmpl $4, -4(%rbp)           /* compara i com 4 */
    jge L2                      /* se i >= 4, sai do loop */

    movq -16(%rbp), %rax        /* rax = a */
    movl -36(%rbp), %edx        /* edx = num */
    movl %edx, (%rax)           /* *a = num */

    addq $4, -16(%rbp)          /* a++ */
    addl $1, -4(%rbp)           /* i++ */

    jmp L1

L2:
    leaq -32(%rbp), %rdi        /* primeiro argumento: local */
    movl $4, %esi               /* segundo argumento: 4 */
    call addl

    leave
    ret
    