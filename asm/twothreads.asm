main:
    call  render
    end

render:
    mov   a, 0      ; i = 0

loop_i:
    cmp   a, 64
    jge   done

    mov   m, a      ; f = ((i - 32) * 752) / 64
    sub   m, 32
    mul   m, 752
    asr   m, 6
    mov   f, m      ; f = ci
    mov   b, 0      ; j = 0

loop_j:
    cmp   b, 160
    jge   next_row

    mov   m, b      ; g = (((j - 80) * 752) / 160) - 160
    sub   m, 80
    mul   m, 752
    sdiv  m, 160
    sub   m, 160
    mov   g, m      ; g = cj

    mov   d, 0      ; x2
    mov   e, 0      ; y2
    mov   c, 0      ; k

loop_k:
    cmp   c, 100
    jge   escape

    mov   m, d      ; xx = (x2 * x2) >> 8
    mul   m, d
    asr   m, 8
    mov   h, m
    mov   m, e      ; yy = (y2 * y2) >> 8
    mul   m, e
    asr   m, 8
    mov   i, m
    mov   j, h      ; c = xx + yy
    add   j, i

    cmp   j, 1024   ; if (c > r)
    jg    escape    ;     break

    mov   k, h      ; z = xx - yy + cj
    sub   k, i
    add   k, g

    mov   m, d      ; y2 = 2 * ((x2 * y2) >> 8) + ci
    mul   m, e
    asr   m, 8
    mul   m, 2
    add   m, f
    mov   e, m
    mov   d, k      ; x2 = z
    inc   c
    jmp   loop_k

escape:
    mov   l, 0
    cmp   c, 100
    jge   color     ; unbounded case

    mov   m, c      ; val = (k % 4) + 1
    mod   m, 4
    inc   m
    mov   l, m

color:
    mov   o, l
    cmp   o, 0
    je    out0
    cmp   o, 1
    je    out1
    cmp   o, 2
    je    out2
    cmp   o, 3
    je    out3
    cmp   o, 4
    je    out4
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
    call nl
    inc   a
    jmp   loop_i

next_col:
    inc   b
    jmp   loop_j

nl:
    msg '
'
    ret

done:
    ret
