.text
main:
    mov r1, 2315
    lea r2, const
    str r1, r2
    ldr r3, r2
    msg 'r3: ', r3, '\n'
    str 1234, r2
    ldr r3, r2
    msg 'r3: ', r3, '\n'
    
    msg 'r2: ', r2, '\n'
    str 5678, 268435456

    ldr r3, 268435456

    msg 'r3: ', r3, '\n'

    str 0xabcd, 4(r2)
    ldr r3, 4(r2)
    msg 'r3: ', r3, '\n'
    halt

.data
const:
    .word 0
    .word 0
    .word 0
    .word 0
