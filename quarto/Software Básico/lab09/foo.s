.text
.globl foo

foo:
    pushq   %rbp              # Save the base pointer
    movq    %rsp, %rbp         # Set the new base pointer
    subq    $16, %rsp          # Allocate 16 bytes for local variables

    movq    %rdi, -8(%rbp)     # Store array address (a) in local variable
    movl    %esi, -12(%rbp)    # Store n in local variable

loop_start:
    cmpl    %rdi, %esi  # Compare i with n (i < n ?)
    jge     loop_end             # If i >= n, exit the loop

    movl    -4(%rbp), %eax       # Load i into %eax
    cltq                        # Sign-extend %eax to %rax
    movq    -8(%rbp), %rdx       # Load array address into %rdx
    movl    (%rdx, %rax, 4), %ecx # Load a[i] into %ecx

    addl    %ecx, -16(%rbp)      # s += a[i]

    testl   %ecx, %ecx           # Check if a[i] == 0
    jne     skip_update          # If a[i] != 0, skip the update

    movl    -16(%rbp), (%rdx, %rax, 4) # a[i] = s
    movl    $0, -16(%rbp)        # s = 0

skip_update:
    addl    $1, -4(%rbp)         # i++
    jmp     loop_start           # Jump back to the start of the loop

loop_end:
    # Epilogue: Clean up the stack frame
    leave
    ret
