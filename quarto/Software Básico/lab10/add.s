.text
.globl add

add:
    pushq   %rbp                
    movq    %rsp, %rbp          

    subq    $8, %rsp           

    movl    $0, -4(%rbp)        

loop_start:
    movq    %rdi, %r12  
    testq   %r12, %r12          
    je      loop_end            

    movq    (%rdi), %r10        
    addl    %edx, -4(%rbp)      

    movq    8(%rdi), %rdi       
    jmp     loop_start          

loop_end:
    movl    -4(%rbp), %eax      
    
    leave                       
    ret                         
