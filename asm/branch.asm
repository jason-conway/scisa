.text
main:
    ; cmp r, r
    mov r0, 1
    mov r1, 10
    mov r2, 10
    cmp r1, r2
    je l0
    mov r0, 0
l0:
    msg 'j ', r0, '\n'

    mov r0, 1
    mov r1, 10
    mov r2, 20
    cmp r1, r2
    jne l1
    mov r0, 0
l1:
    msg 'jne ', r0, '\n'


    mov r0, 1
    mov r1, 20
    mov r2, 10
    cmp r1, r2
    jg l2
    mov r0, 0
l2:
    msg 'jg ', r0, '\n'


    mov r0, 1
    mov r1, 20
    mov r2, 10
    cmp r1, r2
    jge l3
    mov r0, 0
l3:
    msg 'jge ', r0, '\n'


    mov r0, 1
    mov r1, 10
    mov r2, 20
    cmp r1, r2
    jl l4
    mov r0, 0
l4:
    msg 'jl ', r0, '\n'


    mov r0, 1
    mov r1, 10
    mov r2, 20
    cmp r1, r2
    jle l5
    mov r0, 0
l5:
    msg 'jle ', r0, '\n'


    ; cmp r, imm
    mov r0, 1
    mov r1, 10
    cmp r1, 10
    je l6
    mov r0, 0
l6:
    msg 'j ', r0, '\n'

    mov r0, 1
    mov r1, 10
    cmp r1, 20
    jne l7
    mov r0, 0
l7:
    msg 'jne ', r0, '\n'


    mov r0, 1
    mov r1, 20
    cmp r1, 10
    jg l8
    mov r0, 0
l8:
    msg 'jg ', r0, '\n'


    mov r0, 1
    mov r1, 20
    cmp r1, 10
    jge l9
    mov r0, 0
l9:
    msg 'jge ', r0, '\n'


    mov r0, 1
    mov r1, 10
    cmp r1, 20
    jl l10
    mov r0, 0
l10:
    msg 'jl ', r0, '\n'


    mov r0, 1
    mov r1, 10
    cmp r1, 20
    jle l11
    mov r0, 0
l11:
    msg 'jle ', r0, '\n'


    ; cmp imm, r
    mov r0, 1
    mov r2, 10
    cmp 10, r2
    je l12
    mov r0, 0
l12:
    msg 'j ', r0, '\n'

    mov r0, 1
    mov r2, 20
    cmp 10, r2
    jne l13
    mov r0, 0
l13:
    msg 'jne ', r0, '\n'


    mov r0, 1
    mov r2, 10
    cmp 20, r2
    jg l14
    mov r0, 0
l14:
    msg 'jg ', r0, '\n'


    mov r0, 1
    mov r2, 10
    cmp 20, r2
    jge l15
    mov r0, 0
l15:
    msg 'jge ', r0, '\n'


    mov r0, 1
    mov r2, 20
    cmp 10, r2
    jl l16
    mov r0, 0
l16:
    msg 'jl ', r0, '\n'


    mov r0, 1
    mov r2, 20
    cmp 10, r2
    jle l17
    mov r0, 0
l17:
    msg 'jle ', r0, '\n'


    ; cmp imm, imm
    mov r0, 1
    cmp 10, 10
    je l18
    mov r0, 0
l18:
    msg 'j ', r0, '\n'

    mov r0, 1
    cmp 10, 20
    jne l19
    mov r0, 0
l19:
    msg 'jne ', r0, '\n'


    mov r0, 1
    cmp 20, 10
    jg l20
    mov r0, 0
l20:
    msg 'jg ', r0, '\n'


    mov r0, 1
    cmp 20, 10
    jge l21
    mov r0, 0
l21:
    msg 'jge ', r0, '\n'


    mov r0, 1
    cmp 10, 20
    jl l22
    mov r0, 0
l22:
    msg 'jl ', r0, '\n'


    mov r0, 1
    cmp 10, 20
    jle l23
    mov r0, 0
l23:
    msg 'jle ', r0, '\n'

    halt
