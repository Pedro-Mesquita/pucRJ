# double foo2 (float a, float b) {
#  return sin(a) + cos(b);
#   movq    -8(%rbp), %rbx  /* recupera rbx */}

.text
.globl foo2
foo2:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp             # aloca espaço para salvar variáveis

    movss %xmm0, -4(%rbp)        # move a para -4(%rbp)
    movss %xmm1, -8(%rbp)      # move b para -8(%bp)

    cvtss2sd -4(%rbp), %xmm0     # convert float para double
    call sin
    movsd %xmm0, -16(%rbp)      # move sin(a) para -16(%rbp)

    movss -8(%rbp), %xmm0       # move b para %xmm0
    cvtss2sd %xmm0, %xmm0      # converte float para double
    call cos

    addsd -16(%rbp), %xmm0

    leave
    ret


