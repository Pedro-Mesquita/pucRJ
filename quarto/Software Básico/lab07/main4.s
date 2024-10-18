.data
Sf:   .string "%p\n"
list: .int 65, -105, 111, 34

.text
.globl main
main:

  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %rbx, -8(%rbp)
  movq    %r12, -16(%rbp)

  movq  $list, %r12   
  movl $0, %ebx 

L1:
  cmpl  $4, %ebx   
  je L2            

  movq %r12, %rax

  movq    $Sf, %rdi    
  movq    %rax, %rsi   
  movq  $0, %rax
  call  printf         

  addq $4, %r12
  addl $1, %ebx   
  jmp L1               

L2:
  movq  $0, %rax       
  movq  -16(%rbp), %r12 
  movq  -8(%rbp), %rbx  
  leave
  ret      
