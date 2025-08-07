.text
main:
    sub     sp, 48
    mov     r4, r0

    str     r18, 40(sp)
    str     r17, 36(sp)
    str     lr,  44(sp)
    str     r16, 32(sp)

    mov     r17, 6
    mov     r18, 5
l16:
    mov     r5,  r0
    mov     r16, r17
    sub     r16, r4

l17:
    str     r5, 28(sp)
    str     r4, 24(sp)

    call    exec

    ldr     r5, 28(sp)
    ldr     r4, 24(sp)

    inc     r5
    cmp     r16, r5
    jne     l17
    inc     r4
    cmp     r4, r18
    jne     l16

    ldr     lr,  44(sp)
    ldr     r18, 40(sp)
    ldr     r17, 36(sp)
    ldr     r16, 32(sp)

    mov     r2, r0
    add     sp, 48
    halt

ackermann:
    cmp     r4, r0
    jne     l12
    mov     r2, r5
    inc     r2
    ret

l5:
    mov     r5, 1
l4:
    mov     r4,  r16
    cmp     r16, r0
    jeq      l7

l11:
    mov     r16, r4
    dec     r16

    cmp     r5, r0
    jeq      l5
    dec     r5
    call    ackermann

    mov     r5, r2
    jmp     l4
l7:
    ldr     lr,  28(sp)
    ldr     r16, 24(sp)

    mov     r2, r5
    inc     r2

    add     sp, 32
    ret

l12:
    sub     sp,  32
    str     lr,  28(sp)
    str     r16, 24(sp)
    jmp     l11

exec:
    sub     sp, 32
    mov     r3, r4
    mov     r7, r5

    str     lr, 28(sp)

    call     ackermann

    ldr     lr, 28(sp)

    mov     r6, r2
    mov     r5, r7
    mov     r4, r3

    add     sp, 32
    jmp     print

print:
    msg     'ackermann(', r4, ', ', r5, ') = ', r6, '\n'
    ret
