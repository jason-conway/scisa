.text


main:
    msg 'add\n'
    bl usetup
    add r6, r5
    bl print
    bl isetup
    add r6, r5
    bl print

    msg 'sub\n'
    bl usetup
    sub r6, r5
    bl print
    bl isetup
    sub r6, r5
    bl print
    
    msg 'mul\n'
    bl usetup
    mul r6, r5
    bl print
    bl isetup
    mul r6, r5
    bl print

    msg 'div\n'
    bl usetup
    div r6, r5
    bl print
    bl isetup
    div r6, r5
    bl print


    msg 'sdiv\n'
    bl usetup
    sdiv r6, r5
    bl print
    bl isetup
    sdiv r6, r5
    bl print

    msg 'mod\n'
    bl usetup
    mod r6, r5
    bl print
    bl isetup
    mod r6, r5
    bl print

    msg 'smod\n'
    bl usetup
    smod r6, r5
    bl print
    bl isetup
    smod r6, r5
    bl print

    msg 'and\n'
    bl usetup
    and r6, r5
    bl print
    bl isetup
    and r6, r5
    bl print

    msg 'or\n'
    bl usetup
    or r6, r5
    bl print
    bl isetup
    or r6, r5
    bl print

    msg 'xor\n'
    bl usetup
    xor r6, r5
    bl print
    bl isetup
    xor r6, r5
    bl print

    msg 'lsl\n'
    bl usetup
    lsl r6, r5
    bl print
    bl isetup
    lsl r6, r5
    bl print

    msg 'lsr\n'
    bl usetup
    lsr r6, r5
    bl print
    bl isetup
    lsr r6, r5
    bl print

    msg 'asr\n'
    bl usetup
    asr r6, r5
    bl print
    bl isetup
    asr r6, r5
    bl print
    halt

usetup:
    mov r4, 10
    mov r5, 20
    mov r6, r4
    ret

isetup:
    mov r4, 10
    mov r5, -20
    mov r6, r4
    ret

print:
    msg     'op(', r4, ', ', r5, ') = ', r6, '\n'
    ret