.text
main:
    call  render
    halt

render:
    mov   r0, 0      ; i = 0

loop_i:
    cmp   r0, 64
    jge   done

    mov   r12, r0      ; r5 = ((i - 32) * 752) / 64
    sub   r12, 32
    mul   r12, 752
    asr   r12, 6
    mov   r5, r12      ; r5 = ci
    mov   r1, 0      ; j = 0

loop_j:
    cmp   r1, 160
    jge   next_row

    mov   r12, r1      ; r6 = (((j - 80) * 752) / 160) - 160
    sub   r12, 80
    mul   r12, 752
    sdiv  r12, 160
    sub   r12, 160
    mov   r6, r12      ; r6 = cj

    mov   r3, 0      ; x2
    mov   r4, 0      ; y2
    mov   r2, 0      ; k

loop_k:
    cmp   r2, 100
    jge   escape

    mov   r12, r3      ; xx = (x2 * x2) >> 8
    mul   r12, r3
    asr   r12, 8
    mov   r7, r12
    mov   r12, r4      ; yy = (y2 * y2) >> 8
    mul   r12, r4
    asr   r12, 8
    mov   r8, r12
    mov   r9, r7      ; r2 = xx + yy
    add   r9, r8

    cmp   r9, 1024   ; if (r2 > r)
    jgt   escape    ;     break

    mov   r10, r7      ; z = xx - yy + cj
    sub   r10, r8
    add   r10, r6

    mov   r12, r3      ; y2 = 2 * ((x2 * y2) >> 8) + ci
    mul   r12, r4
    asr   r12, 8
    mul   r12, 2
    add   r12, r5
    mov   r4, r12
    mov   r3, r10      ; x2 = z
    inc   r2
    jmp   loop_k

escape:
    mov   r11, 0
    cmp   r2, 100
    jge   color     ; unbounded case

    mov   r12, r2      ; val = (k % 4) + 1
    mod   r12, 4
    inc   r12
    mov   r11, r12

color:
    mov   r13, r11
    cmp   r13, 0
    jeq   out0
    cmp   r13, 1
    jeq   out1
    cmp   r13, 2
    jeq   out2
    cmp   r13, 3
    jeq   out3
    cmp   r13, 4
    jeq   out4
    jmp   next_col

out0:
    msg   'X'
    jmp   next_col
out1:
    msg   '⋅'
    jmp   next_col
out2:
    msg   '·'
    jmp   next_col
out3:
    msg   '∙'
    jmp   next_col
out4:
    msg   '•'
    jmp   next_col

next_row:
    msg   '\n'
    inc   r0
    jmp   loop_i

next_col:
    inc   r1
    jmp   loop_j

done:
    ret
