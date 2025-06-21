main:
    msg 'primes:\n'

    mov p, 900000
iter:
    cmp p, 1000000
    jl cont
    halt
cont:
    mov x, p
    call is_prime
    cmp y, 0
    je skip
    msg p, '\n'
skip:
    inc p
    jmp iter


is_prime:
    mov b, 0
    cmp x, 1
    je ret_false
    cmp x, 2
    je ret_true

    mov i, 2
loop:
    mov j, i
    mul j, i
    cmp j, x
    jg check
    
    mov z, x
    mod z, i
    cmp z, 0
    jne not
    inc b
not:
    inc i
    jmp loop
check:
    cmp b, 0
    jg ret_false
    jmp ret_true

ret_false:
    mov y, 0
    ret
ret_true:
    mov y, 1
    ret

