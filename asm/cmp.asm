.text
main:
    mov r4, 5
    mov r5, 10
    cmp r4, r5
    msg 'cmp ', cc, '\n'
    mov cc, 0

    cmp r4, 10
    msg 'cmp ', cc, '\n'
    mov cc, 0

    cmp 5, r5
    msg 'cmp ', cc, '\n'
    mov cc, 0

    cmp 5, 10
    msg 'cmp ', cc, '\n'
    mov cc, 0

    halt
