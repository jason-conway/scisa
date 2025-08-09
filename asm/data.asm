.data
.align 4
c1: 
    .word 0x80000000
c2: 
    .word -0x7fffffff

c3: .byte 17
c4: .byte 38
c5: .zero 128

.text
main:
    lea r0, c1
    ldw r1, r0
    msg 'c1: ', r1, ' @ ', r0, '\n'
    
    lea r0, c2
    ldw r1, r0
    msg 'c2: ', r1, ' @ ', r0, '\n'

    lea r0, c3
    ldw r1, r0
    ;and r1, 0xff
    msg 'c3: ', r1, ' @ ', r0, '\n'

    lea r0, c4
    ldw r1, r0
    ;and r1, 0xff
    msg 'c4: ', r1, ' @ ', r0, '\n'

    halt
