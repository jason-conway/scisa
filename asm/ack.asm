.text
main:
    sub     sp, 48
    mov     r4, r0

    mov     r31, sp
    add     r31, 40
    stw     r18, r31

    mov     r31, sp
    add     r31, 40
    stw     r17, r31

    mov     r31, sp
    add     r31, 44
    stw     lr,  r31

    mov     r31, sp
    add     r31, 32
    stw     r16, r31

    mov     r17, 17 ; 6
    mov     r18, 4  ; 5
l16:
    mov     r5,  r0
    mov     r16, r17
    sub     r16, r4

l17:
    mov     r31, sp
    add     r31, 28
    stw     r5,  r31

    mov     r31, sp
    add     r31, 24
    stw     r4,  r31

    call    exec

    mov     r31, sp
    add     r31, 28
    ldw     r5,  r31

    mov     r31, sp
    add     r31, 24
    ldw     r4,  r31

    inc     r5
    cmp     r16, r5
    jne     l17
    inc     r4
    cmp     r4, r18
    jne     l16

    mov     r31, sp
    add     r31, 44
    ldw     lr, r31

    mov     r31, sp
    add     r31, 40
    ldw     r18, r31

    mov     r31, sp
    add     r31, 36
    ldw     r17, r31

    mov     r31, sp
    add     r31, 32
    ldw     r16, r31

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
    jeq     l7

l11:
    mov     r16, r4
    dec     r16

    cmp     r5, r0
    jeq     l5
    dec     r5
    call    ackermann

    mov     r5, r2
    jmp     l4
l7:
    mov     r31, sp
    add     r31, 28
    ldw     lr,  r31

    mov     r31, sp
    add     r31, 24
    ldw     r16, r31

    mov     r2, r5
    inc     r2

    add     sp, 32
    ret

l12:
    sub     sp, 32

    mov     r31, sp
    add     r31, 28
    stw     lr,  r31

    mov     r31, sp
    add     r31, 24
    stw     r16, r31
    jmp     l11

exec:
    sub     sp, 32
    mov     r3, r4
    mov     r7, r5

    mov     r31, sp
    add     r31, 28
    stw     lr, r31

    call     ackermann

    mov     r31, sp
    add     r31, 28
    ldw     lr, r31

    mov     r6, r2
    mov     r5, r7
    mov     r4, r3

    add     sp, 32
    jmp     print

print:
    msg     'ackermann(', r4, ', ', r5, ') = ', r6, '\n'
    ret
