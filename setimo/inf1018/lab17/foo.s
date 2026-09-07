# double foo (double a, int b) {
#  return a * (-b);
# }

.text
.globl foo
foo:
    imul $-1, %esi
    cvtsi2sd %esi, %xmm1

    mulsd %xmm1, %xmm0 

    ret