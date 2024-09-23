  .data
  Sf: .string "ls -ls"

  .text
  .globl main
  main:
  /* prologo */
    pushq %rbp
    movq  %rsp, %rbp

  /* coloque seu codigo aqui */
    movq $Sf, %rdi  
    call system    
    movl $0, %eax

   /* finalizacao */
    leave
    ret
