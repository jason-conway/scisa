.text
main:
    lea r0, (lstr)
    msg 'loaded lstr address into r0\n'
    lea r1, (lstrsz)
    msg 'loaded lstrsz address into r1\n'
    ldw r2, (r1)
    msg '*lstrsz: ', r2, '\n'
    ldub r10, (r0)
    msg '*r0: ', r10, '\n'
    out (r0), r2
    msg '\n'
    lea r0, (lascii)
    out (r0), r2
    msg '\n'
    msg 'done\n'
    halt

.data
lstr:
    .align 1
    .byte	72
    .byte	101
    .byte	108
    .byte	108
    .byte	111
    .byte	44
    .byte	32
    .byte	87
    .byte	111
    .byte	114
    .byte	108
    .byte	100
    .byte	33
    .byte	0
lstrsz:
    .align 4
    .word 14

lascii:
    .ascii 'Hello, World!'

