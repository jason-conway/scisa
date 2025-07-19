#pragma once

#pragma region OPCODES
#define OPCODE_LIST \
    abort,   \
    incr,    \
    decr,    \
    negr,    \
    pushr,   \
    popr,    \
    movri,   \
    movrr,   \
    addri,   \
    addrr,   \
    saddri,  \
    saddrr,  \
    subri,   \
    subrr,   \
    mulri,   \
    mulrr,   \
    divri,   \
    divrr,   \
    sdivri,  \
    sdivrr,  \
    modri,   \
    modrr,   \
    smodri,  \
    smodrr,  \
    andri,   \
    andrr,   \
    orri,    \
    orrr,    \
    xorri,   \
    xorrr,   \
    lslri,   \
    lslrr,   \
    lsrri,   \
    lsrrr,   \
    asrri,   \
    asrrr,   \
    ldrri,   \
    ldrrr,   \
    ldrrir,  \
    strii,   \
    strir,   \
    striir,  \
    strri,   \
    strrr,   \
    strrir,  \
    learl,   \
    learil,  \
    cmpii,   \
    cmpir,   \
    cmpri,   \
    cmprr,   \
    jmp,     \
    jne,     \
    je,      \
    jge,     \
    jg,      \
    jle,     \
    jl,      \
    call,    \
    ret,     \
    msg,     \
    halt
#pragma endregion

#pragma region MNEMONICS
#define MNEMONICS_LIST \
    inc,    \
    dec,    \
    neg,    \
    push,   \
    pop,    \
    ret,    \
    halt,   \
    mov,    \
    add,    \
    sadd,   \
    sub,    \
    mul,    \
    div,    \
    sdiv,   \
    mod,    \
    smod,   \
    and,    \
    or,     \
    xor,    \
    lsl,    \
    lsr,    \
    asr,    \
    ldr,    \
    str,    \
    lea,    \
    cmp,    \
    jmp,    \
    jne,    \
    je,     \
    jge,    \
    jg,     \
    jle,    \
    jl,     \
    call,   \
    msg,
#pragma endregion

#pragma region REGS
#define REGISTER_LIST \
    r0,     \
    r1,     \
    r2,     \
    r3,     \
    r4,     \
    r5,     \
    r6,     \
    r7,     \
    r8,     \
    r9,     \
    r10,    \
    r11,    \
    r12,    \
    r13,    \
    r14,    \
    r15,    \
    r16,    \
    r17,    \
    r18,    \
    r19,    \
    r20,    \
    r21,    \
    r22,    \
    r23,    \
    r24,    \
    r25,    \
    r26,    \
    r27,    \
    r28,    \
    r29,    \
    r30,    \
    r31,    \
    pc,     \
    lr,     \
    fp,     \
    sp,     \
    cc
#pragma endregion

#pragma region ASM DIRECTIVES
#define DIRECTIVE_LIST \
    text, \
    data, \
    word, \
    byte
#pragma endregion