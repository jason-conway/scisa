.data
.align 4
masks:
    .word 0x55555555
    .word 0x33333333
    .word 0x0f0f0f0f

.text
main:
    sub   sp, 32
    str   lr, 28(sp)
    mov   r4, 16 ; compute popcount for inputs 1 through this value
    call  verify
    ldr   lr, 28(sp)
    add   sp, 32
    halt

verify:
    sub   sp,  40
    str   lr,  36(sp)
    str   r17, 32(sp)
    str   r16, 28(sp)

    mov   r17, r4 ; r17 == max
    mov   r16, 1
cont:
    mov   r4, r16
    call  popcount
    mov   r5, r2
    mov   r4, r16 ; reload for disp
    msg   'popcount(', r4, ')\t= ', r5, '\n'
    
    inc   r16
    cmp   r16, r17
    jlt   cont

    ldr   lr,  36(sp)
    ldr   r17, 32(sp)
    ldr   r16, 28(sp)
    add   sp,  40
    ret

popcount:
    lea   r28, masks

    mov   r2, r4
    lsr   r2, 1
    ldr   r3, 0(r28)
    and   r2, r3
    sub   r4, r2
    ldr   r3, 4(r28)
    mov   r2, r4
    and   r2, r3
    lsr   r4, 2
    and   r4, r3
    add   r2, r4
    mov   r3, r2
    lsr   r3, 4
    add   r3, r2
    ldr   r2, 8(r28)
    and   r3, r2
    mov   r2, r3
    lsl   r2, 8
    add   r2, r3
    mov   r3, r2
    lsl   r3, 16
    add   r2, r3
    lsr   r2, 24

    ret
