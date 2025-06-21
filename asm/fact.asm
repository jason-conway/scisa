main:
    mov   r0, 12
    mov   r1, r0
    mov   r2, r0
    call  proc_fact
    call  print
    halt

proc_fact:
    dec   r1
    mul   r2, r1
    cmp   r1, 1
    jne   proc_fact
    ret

print:
    msg    r0, '! = ', r2, '\n'
    ret
