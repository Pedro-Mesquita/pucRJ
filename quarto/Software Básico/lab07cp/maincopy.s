.data
S2: .int 65, 108, 111, 0 /* Use .int for integers */
Sf: .string "%p\n"  /* Format string for printing integers */
Sf2: .string "\n"   /* Newline string */

.text
.globl main
main:

/********************************************************/
/* Prologue */
  pushq   %rbp
  movq    %rsp, %rbp
  subq    $16, %rsp
  movq    %rbx, -8(%rbp)  /* Save rbx */
  movq    %r12, -16(%rbp)  /* Save r12 */
/********************************************************/

  movq  $S2, %r12  

L1:
  movl  (%r12), %eax  
  cmp   $0, %eax       
  je L2           

  /*************************************************************/
  /* Print the integer */
  movq    $Sf, %rdi  /* First parameter: pointer to format string */
  movq    %r12, %rsi /* Second parameter: integer to print */
  movl    $0, %eax    /* Clear eax for variadic function */
  call    printf       /* Call printf */
  /*************************************************************/

  addq   $4, %r12     
  jmp L1             

L2:  
  /*************************************************************/
  /* Print a newline */
  movq    $Sf2, %rdi  /* Pointer to newline string */
  movl    $0, %eax    /* Clear eax for variadic function */
  call    printf       /* Call printf */
  /*************************************************************/

/***************************************************************/
/* Epilogue */
  movq  $0, %rax      /* Return value 0 */
  movq    -16(%rbp), %r12 /* Restore r12 */
  movq    -8(%rbp), %rbx  /* Restore rbx */
  leave
  ret      
/***************************************************************/
