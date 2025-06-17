main:
    msg 'primes:\n'

    mov r1, 0
iter:
    cmp r1, 1000000
    jl cont
    halt
cont:
    mov r3, r1
    call is_prime
    cmp r0, 0
    je skip
    msg r1, '\n'
skip:
    inc r1
    jmp iter

is_prime:
    mov r2, 0
    cmp r3, 1
    je ret_false
    cmp r3, 2
    je ret_true

    mov r5, 2
loop:
    mov r4, r5
    mul r4, r5
    cmp r4, r3
    jg check
    
    mov r6, r3
    mod r6, r5
    cmp r6, 0
    jne not
    inc r2
not:
    inc r5
    jmp loop
check:
    cmp r2, 0
    jg ret_false
    jmp ret_true

ret_false:
    mov r0, 0
    ret
ret_true:
    mov r0, 1
    ret

