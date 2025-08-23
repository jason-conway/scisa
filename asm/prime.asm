.text
main:
    msg 'no.\tprime\n'
    mov r7, 1
    mov r1, 0
iter:
    cmp r1, 1000000
    blt cont
    msg '\n'
    halt
cont:
    mov r3, r1
    bl is_prime
    cmp r0, 0
    beq skip
    msg '\r'
    msg r7, '\t', r1
    msg '\0'
    inc r7
skip:
    inc r1
    b iter

is_prime:
    mov r2, 0
    cmp r3, 1
    beq ret_false
    cmp r3, 2
    beq ret_true

    mov r5, 2
loop:
    mov r4, r5
    mul r4, r5
    cmp r4, r3
    bgt check
    
    mov r6, r3
    mod r6, r5
    cmp r6, 0
    bne not
    inc r2
not:
    inc r5
    b loop
check:
    cmp r2, 0
    bgt ret_false
    b ret_true

ret_false:
    mov r0, 0
    ret
ret_true:
    mov r0, 1
    ret

