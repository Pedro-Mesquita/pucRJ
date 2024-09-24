.data
nums: .int 3, -5, 7, 8, -2
lim:  .int 1             
s1:   .string "%d\n"       

.text
.globl main

main:
  pushq %rbp
  movq  %rsp, %rbp
  subq  $16, %rsp

  movq $0, %rbx            
  movq $nums, %r12         

L1:
  cmpq $5, %rbx            
  jge L2                   

  movl (%r12), %edi 
  movl lim, %esi          
  call filtro             

  movl %eax, %esi          
  movq $s1, %rdi           
  movl $0, %eax          
  call printf            

  addq $1, %rbx                
  addq $4, %r12                
  jmp L1                   

L2:
  movq %rbp, %rsp
  popq %rbp
  ret
