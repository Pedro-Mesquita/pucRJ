.text
.globl fat

fat:
/* Início */
    pushq %rbp
    movq %rsp, %rbp

    movl %edi, %eax      

/* Caso Base */
    cmpl $1, %eax        
    jle base_case         

/* Passo Recursivo */
    pushq %rdi            
    
    subl $1, %edi         
    call fat             
    
    popq %rdi            
    
    imull %edi, %eax     
    jmp end              

/* Caso base: n <= 1 */
base_case:
    movl $1, %eax         

/* Termina */
end:
    leave              
    ret                
