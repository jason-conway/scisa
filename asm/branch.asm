.text
main:
    ; cmp r, r
    mov r0, 1
    mov r1, 10
    mov r2, 10
    cmp r1, r2
    beq l0
    mov r0, 0
l0:
    msg 'beq ', r0, '\n'

    mov r0, 1
    mov r1, 10
    mov r2, 20
    cmp r1, r2
    bne l1
    mov r0, 0
l1:
    msg 'bne ', r0, '\n'


    mov r0, 1
    mov r1, 20
    mov r2, 10
    cmp r1, r2
    bgt l2
    mov r0, 0
l2:
    msg 'bgt ', r0, '\n'


    mov r0, 1
    mov r1, 20
    mov r2, 10
    cmp r1, r2
    bge l3
    mov r0, 0
l3:
    msg 'bge ', r0, '\n'


    mov r0, 1
    mov r1, 10
    mov r2, 20
    cmp r1, r2
    blt l4
    mov r0, 0
l4:
    msg 'blt ', r0, '\n'


    mov r0, 1
    mov r1, 10
    mov r2, 20
    cmp r1, r2
    ble l5
    mov r0, 0
l5:
    msg 'ble ', r0, '\n'


    ; cmp r, imm
    mov r0, 1
    mov r1, 10
    cmp r1, 10
    beq l6
    mov r0, 0
l6:
    msg 'beq ', r0, '\n'

    mov r0, 1
    mov r1, 10
    cmp r1, 20
    bne l7
    mov r0, 0
l7:
    msg 'bne ', r0, '\n'


    mov r0, 1
    mov r1, 20
    cmp r1, 10
    bgt l8
    mov r0, 0
l8:
    msg 'bgt ', r0, '\n'


    mov r0, 1
    mov r1, 20
    cmp r1, 10
    bge l9
    mov r0, 0
l9:
    msg 'bge ', r0, '\n'


    mov r0, 1
    mov r1, 10
    cmp r1, 20
    blt l10
    mov r0, 0
l10:
    msg 'blt ', r0, '\n'


    mov r0, 1
    mov r1, 10
    cmp r1, 20
    ble l11
    mov r0, 0
l11:
    msg 'ble ', r0, '\n'


    ; cmp imm, r
    mov r0, 1
    mov r2, 10
    cmp 10, r2
    beq l12
    mov r0, 0
l12:
    msg 'beq ', r0, '\n'

    mov r0, 1
    mov r2, 20
    cmp 10, r2
    bne l13
    mov r0, 0
l13:
    msg 'bne ', r0, '\n'


    mov r0, 1
    mov r2, 10
    cmp 20, r2
    bgt l14
    mov r0, 0
l14:
    msg 'bgt ', r0, '\n'


    mov r0, 1
    mov r2, 10
    cmp 20, r2
    bge l15
    mov r0, 0
l15:
    msg 'bge ', r0, '\n'


    mov r0, 1
    mov r2, 20
    cmp 10, r2
    blt l16
    mov r0, 0
l16:
    msg 'blt ', r0, '\n'


    mov r0, 1
    mov r2, 20
    cmp 10, r2
    ble l17
    mov r0, 0
l17:
    msg 'ble ', r0, '\n'


    ; cmp imm, imm
    mov r0, 1
    cmp 10, 10
    beq l18
    mov r0, 0
l18:
    msg 'beq ', r0, '\n'

    mov r0, 1
    cmp 10, 20
    bne l19
    mov r0, 0
l19:
    msg 'bne ', r0, '\n'


    mov r0, 1
    cmp 20, 10
    bgt l20
    mov r0, 0
l20:
    msg 'bgt ', r0, '\n'


    mov r0, 1
    cmp 20, 10
    bge l21
    mov r0, 0
l21:
    msg 'bge ', r0, '\n'


    mov r0, 1
    cmp 10, 20
    blt l22
    mov r0, 0
l22:
    msg 'blt ', r0, '\n'


    mov r0, 1
    cmp 10, 20
    ble l23
    mov r0, 0
l23:
    msg 'ble ', r0, '\n'

    halt
