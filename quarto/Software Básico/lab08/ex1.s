  .data
  s1: .string "ls -ls"

  .text
  .globl main
  main:
  /* prologo */
    pushq %rbp
    movq  %rsp, %rbp

    movq $s1, %rdi
    call system

  /* finalizacao */
    leave
    ret
    