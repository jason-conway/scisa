main:
    mov r1, 0
    mov r2, 0x0000 ; starting address
iter_str:
    cmp r1, 0x1000 ; end count
    jl cont_str
    mov r1, 0
    mov r2, 0
    jmp iter_ldr
cont_str:
    msg 'mem[', r2, '] = ', r1, '\n'
    str r1, r2 ; store low byte of r1 in the address contained in r2
    add r2, 4
    inc r1
    jmp iter_str
iter_ldr:
    cmp r1, 0xfff ; end count
    jl cont_ldr
    halt
cont_ldr:
    ldr r1, r2 ; load value at the address contained in r2 into r1
    msg r1, ' = mem[', r2, ']\n'
    add r2, 4
    jmp iter_ldr
