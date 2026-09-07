//float foo (double a, float b) {
//  return (a+b)*(a-b);
//}

.text 
.globl foo
foo: 
    cvtsd2ss %xmm0, %xmm0 # xm0 = float(a)
    movss %xmm0, %xmm2    # xmm2 = a
    movss %xmm1, %xmm3    # xmm3 = b

    addss %xmm1, %xmm0    # xmm0 = a+b
    subss %xmm3, %xmm2    # xmm2 = a-b

    mulss %xmm2, %xmm0    # (a+b)*(a-b)

    ret
    
