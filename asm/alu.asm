.text


main:
    msg 'add\n'
    call usetup
    add r6, r5
    call print
    call isetup
    add r6, r5
    call print

    msg 'sadd\n'
    call usetup
    sadd r6, r5
    call print
    call isetup
    sadd r6, r5
    call print

    msg 'sub\n'
    call usetup
    sub r6, r5
    call print
    call isetup
    sub r6, r5
    call print
    
    msg 'mul\n'
    call usetup
    mul r6, r5
    call print
    call isetup
    mul r6, r5
    call print

    msg 'div\n'
    call usetup
    div r6, r5
    call print
    call isetup
    div r6, r5
    call print


    msg 'sdiv\n'
    call usetup
    sdiv r6, r5
    call print
    call isetup
    sdiv r6, r5
    call print

    msg 'mod\n'
    call usetup
    mod r6, r5
    call print
    call isetup
    mod r6, r5
    call print

    msg 'smod\n'
    call usetup
    smod r6, r5
    call print
    call isetup
    smod r6, r5
    call print

    msg 'and\n'
    call usetup
    and r6, r5
    call print
    call isetup
    and r6, r5
    call print

    msg 'or\n'
    call usetup
    or r6, r5
    call print
    call isetup
    or r6, r5
    call print

    msg 'xor\n'
    call usetup
    xor r6, r5
    call print
    call isetup
    xor r6, r5
    call print

    msg 'lsl\n'
    call usetup
    lsl r6, r5
    call print
    call isetup
    lsl r6, r5
    call print

    msg 'lsr\n'
    call usetup
    lsr r6, r5
    call print
    call isetup
    lsr r6, r5
    call print

    msg 'asr\n'
    call usetup
    asr r6, r5
    call print
    call isetup
    asr r6, r5
    call print
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