.text
main:

    cmp 0, 0
    msg 'cmp ', cc, '\n'
    mov cc, 0

    cmp 1, 0
    msg 'cmp ', cc, '\n'
    mov cc, 0

    ldw r0, -4(r16)
    lea r2, 0(func)
    b label
    b 
    cmp 0, 1
    msg 'cmp ', cc, '\n'
    mov cc, 0
    halt

