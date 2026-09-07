#float foo1 (float a, float b) {
#  return sin(a) + b;
#}

.text
.globl foo1
foo1:
    subq $16, %rsp                      # Aloca 16 bytes em rsp
    movss %xmm1, (%rsp)                 # Salva b no espaço alocado
    cvtss2sd %xmm0, %xmm0               # (double)a
    
    call sin                            # chama sin passando a

    cvtsd2ss %xmm0, %xmm0               # converte de double para float
    addss (%rsp), %xmm0                 # adicionado float no float

    addq $16, %rsp

    ret                                 # retorna %xm00
