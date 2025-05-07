.text
.globl map2

map2:
    pushq %rbp              
    movq %rsp, %rbp         

    subq $16, %rsp          
    movq %r12, -8(%rbp)     
    movq %r11, -16(%rbp)    

    movq %rdi, %r11         
    movq %rsi, %r10         
    movl $0, %r12d          

loop:
    cmp %edx, %r12d         
    jae finaliza            

    movl (%r11,%r12,4), %edi 
    call f                  
    movl %eax, (%r10,%r12,4) 

    inc %r12d               
    jmp loop                

finaliza:
    movq -8(%rbp), %r12     
    movq -16(%rbp), %r11    

    leave                   
    ret                     
