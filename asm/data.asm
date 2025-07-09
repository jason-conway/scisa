.text
main:
    lea r0, array0
    msg r0, '\n'
    lea r0, 0(array0)
    msg r0, '\n\n'

    lea r0, 0(array0)
    ldr r1, r0
    msg r1, '\n'

    lea r0, 4(array0)
    ldr r1, r0
    msg r1, '\n'

    lea r0, 8(array0)
    ldr r1, r0
    msg r1, '\n'

    lea r0, 12(array0)
    ldr r1, r0
    msg r1, '\n'

    lea r0, 16(array0)
    ldr r1, r0
    msg r1, '\n'

    lea r0, array0
    
    ldr r1, 0(r0)
    msg r1, '\n'
    ldr r1, 4(r0)
    msg r1, '\n'
    ldr r1, 8(r0)
    msg r1, '\n'
    ldr r1, 12(r0)
    msg r1, '\n'
    ldr r1, 16(r0)
    msg r1, '\n'
    
    ldr r1, 0x00(r0)
    msg r1, '\n'
    ldr r1, 0x04(r0)
    msg r1, '\n'
    ldr r1, 0x08(r0)
    msg r1, '\n'
    ldr r1, 0x0c(r0)
    msg r1, '\n'
    ldr r1, 0x10(r0)
    msg r1, '\n'

    mov r1, 10
    str r1, 0x00(r0)
    mov r1, 0
    ldr r1, 0x00(r0)
    msg r1, '\n'

    halt

.data
array0:
    .word 0x1000
    .word 0x2000
    .word 0x3000
    .word 0x4000
    .word 0x5000
