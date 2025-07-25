.text
main:

    cmp 0, 0
    msg 'cmp ', cc, '\n'
    mov cc, 0

    cmp 1, 0
    msg 'cmp ', cc, '\n'
    mov cc, 0

    cmp 0, 1
    msg 'cmp ', cc, '\n'
    mov cc, 0
    halt

