foo: 
    pushq %rbp
    movq %rsp, %rbp

    subq $32, %rsp

    leaq -32(%rbp), %rdi
    leaq -16(%rbp), %rsi

    call boo

    leave 
    ret