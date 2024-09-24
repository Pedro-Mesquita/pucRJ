.data
Sf:   .string "%d\n"

.text
.globl main
main:

  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %rbx, -8(%rbp)
  movq    %r12, -16(%rbp)

  movl  $1, %r12d   

L1:
  cmpl  $11, %r12d  
  jge L2            

  movl %r12d, %eax
  imull %eax, %eax

  movq    $Sf, %rdi    
  movl    %eax, %esi   
  movl  $0, %eax
  call  printf         

  addl $1, %r12d       
  jmp L1               

L2:
  movq  $0, %rax       
  movq  -16(%rbp), %r12 
  movq  -8(%rbp), %rbx  
  leave
  ret      
