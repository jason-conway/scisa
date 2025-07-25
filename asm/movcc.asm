.text
main:
    mov r0, 0
    mov r1, 6
    mov r2, 5
    cmp r1, r2
    movne r0, 1
    msg 'r0 ', r0, '\n'

    mov r0, 0
    mov r1, 10
    mov r2, 20
    mov r10, 30
    cmp r1, r2
    moveq r0, r10
    msg 'r0 ', r0, '\n'

    mov r0, 0
    mov r1, 10
    mov r2, 20
    mov r10, 30
    cmp r1, r2
    movge r0, r10
    msg 'r0 ', r0, '\n'

    mov r0, 0
    mov r1, 10
    mov r2, 20
    mov r10, 30
    cmp r1, r2
    movle r0, r10
    msg 'r0 ', r0, '\n'

    halt
