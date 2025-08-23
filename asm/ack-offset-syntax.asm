.text
main:
    sub     sp, 48
    mov     r4, r0

    stw     r18, 40(sp)
    stw     r17, 36(sp)
    stw     lr,  44(sp)
    stw     r16, 32(sp)

    mov     r17, 6
    mov     r18, 5
l16:
    mov     r5,  r0
    mov     r16, r17
    sub     r16, r4

l17:
    stw     r5, 28(sp)
    stw     r4, 24(sp)

    call    exec

    ldw     r5, 28(sp)
    ldw     r4, 24(sp)

    inc     r5
    cmp     r16, r5
    bne     l17
    inc     r4
    cmp     r4, r18
    bne     l16

    ldw     lr,  44(sp)
    ldw     r18, 40(sp)
    ldw     r17, 36(sp)
    ldw     r16, 32(sp)

    mov     r2, r0
    add     sp, 48
    halt

ackermann:
    cmp     r4, r0
    bne     l12
    mov     r2, r5
    inc     r2
    ret

l5:
    mov     r5, 1
l4:
    mov     r4,  r16
    cmp     r16, r0
    beq      l7

l11:
    mov     r16, r4
    dec     r16

    cmp     r5, r0
    beq      l5
    dec     r5
    call    ackermann

    mov     r5, r2
    b     l4
l7:
    ldw     lr,  28(sp)
    ldw     r16, 24(sp)

    mov     r2, r5
    inc     r2

    add     sp, 32
    ret

l12:
    sub     sp,  32
    stw     lr,  28(sp)
    stw     r16, 24(sp)
    b     l11

exec:
    sub     sp, 32
    mov     r3, r4
    mov     r7, r5

    stw     lr, 28(sp)

    call     ackermann

    ldw     lr, 28(sp)

    mov     r6, r2
    mov     r5, r7
    mov     r4, r3

    add     sp, 32
    b     print

print:
    msg     'ackermann(', r4, ', ', r5, ') = ', r6, '\t[cyc: ', cyc, ']\n'
    ret
