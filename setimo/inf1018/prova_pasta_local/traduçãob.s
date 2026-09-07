/*
struct exam { float val; int tipo; };
double inv (double val);
float boo (double d, float bias) {
  int i;
  struct exam temp[5];
  for (i=0; i<5; i++) {
    temp[i].val = inv(d);
    temp[i].tipo = i;
  }
  return temp[0].val + bias;
}
*/

.text
.globl boo
boo:
  pushq %rbp
  movq %rsp, %rbp
  subq $64, %rsp

  movsd %xmm0, -24(%rbp)               # double d
  movss %xmm1, -16(%rbp)               # float bias
  movl $0, -12(%rbp)                   # int i

  L1:
    cmpl $5, -12(%rbp)                 # compara 5 com i
    jae L2                             # se for maior ou igual pula para L2

    movsd -24(%rbp), %xmm0             # move d para xmm0
    call inv                           # chama a função inv passando xmm0

    movslq -12(%rbp), %r10             # estende i para 64 bits
    movss %xmm0, -64(%rbp, %r10, 8)    # atribui o retorno de inv para a prop val da structure
    movl -12(%rbp), -60(%rbp, %r10, 8) # atribui o valor do iterador para a prop tipo da structure

    addl $1, -12(%rbp)                 # adiciona 1 no iterador
    jmp L1                             # repete o loop
  L2:
    movss -16(%rbp), %xmm0             # move o valor do bias para o registrador de retorno
    addss -64(%rbp), %xmm0             # adiciona o valor strucutre.val no registrador de retorno

    leave                              # limpa a pilha
    ret                                # retorna a variável