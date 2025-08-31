<!-- markdownlint-disable MD024 MD033 MD041 MD045 -->

# `scisa`: Sweetened Condensed Instruction Set Architecture

`scisa` is a losely defined ISA and an assembly interpreter for it.

---

[<img align="center" src="./etc/scisa.png"/>](etc/scisa.png)

---

## SCISA VM

### Registers

| register   | description               |
| ---------- | ------------------------- |
| `r0`–`r31` | general-purpose registers |
| `pc`       | program counter           |
| `lr`       | link register             |
| `sp`       | stack pointer             |
| `fp`       | frame pointer             |
| `cc`       | 3-way comparison flag     |
| `cyc`      | cycle counter             |

### Stack

The stack is 2MB in size, with the top of the stack at `0x7fffffff`.

| size | high address | low address  |
| ---- | ------------ | ------------ |
| 2MB  | `0x7fffffff` | `0x7fdfffff` |

### Data Segment

The data segment is dynamically sized, with a base address of `0x10000000`.

## The SCISA Instruction Set

`scisa` implements a sweet (and condensed) set of instructions. The ISA intends
to be small without being constraining. Instructions are being added as needed.

### Arithmetic and Logic Instructions (Unary)

| Mnemonic | Operands           | Operation                           |
| -------- | ------------------ | ----------------------------------- |
| `add`    | `reg, reg/imm`     | `rd += (unsigned)rs/imm`            |
| `sub`    | `reg, reg/imm`     | `rd -= (unsigned)rs/imm`            |
| `mul`    | `reg, reg/imm`     | `rd *= (unsigned)rs/imm`            |
| `div`    | `reg, reg/imm`     | `rd /= (unsigned)rs/imm`            |
| `sdiv`   | `reg, reg/imm`     | `rd /= (signed)rs/imm`              |
| `mod`    | `reg, reg/imm`     | `rd %= (unsigned)rs/imm`            |
| `smod`   | `reg, reg/imm`     | `rd %= (signed)rs/imm`              |
| `and`    | `reg, reg/imm`     | `(unsigned)rd &= (unsigned)rs/imm`  |
| `or`     | `reg, reg/imm`     | `(unsigned)rd \|= (unsigned)rs/imm` |
| `xor`    | `reg, reg/imm`     | `(unsigned)rd ^= (unsigned)rs/imm`  |
| `lsl`    | `reg, reg/imm`     | `rd <<= (unsigned)rs/imm & mask`    |
| `lsr`    | `reg, reg/imm`     | `rd >>= (unsigned)rs/imm & mask`    |
| `asr`    | `reg, reg/imm`     | `(signed)rd >>= rs/imm & mask`      |
| `inc`    | `reg`              | `rd++`                              |
| `dec`    | `reg`              | `rd--`                              |
| `neg`    | `reg`              | `rd = -rd`                          |
| `cmp`    | `reg/imm, reg/imm` | see section on  `cmp`               |

#### Notes

- Shift operations (`lsl`, `lsr`, `asr`) mask of `0x1f`
- The signed modulo instruction (`smod`) uses truncated division, matching the
  behavior of languages like C
- `cmp` *does* allow `imm/imm` comparison

### Control Flow Instructions

| Mnemonic | Operands | Operation                     |
| -------- | -------- | ----------------------------- |
| `b`      | `label`  | `pc = &label`                 |
| `bl`     | `label`  | `lr = pc; pc = &label`        |
| `bne`    | `label`  | `if (cc & CC_NE) pc = &label` |
| `beq`    | `label`  | `if (cc & CC_EQ) pc = &label` |
| `bge`    | `label`  | `if (cc & CC_GE) pc = &label` |
| `bgt`    | `label`  | `if (cc & CC_GT) pc = &label` |
| `ble`    | `label`  | `if (cc & CC_LE) pc = &label` |
| `blt`    | `label`  | `if (cc & CC_LT) pc = &label` |
| `bhs`    | `label`  | `if (cc & CC_HS) pc = &label` |
| `bhi`    | `label`  | `if (cc & CC_HI) pc = &label` |
| `bls`    | `label`  | `if (cc & CC_LS) pc = &label` |
| `blo`    | `label`  | `if (cc & CC_LO) pc = &label` |
| `br`     | `(reg)`  | `pc = *rs`                  |
| `blr`    | `(reg)`  | `lr = pc; pc = *rs`         |
| `ret`    |          | `pc = lr`                     |
| `halt`   |          | `halt successfully`           |

#### Notes

- All programs must end with a halt instruction

### Data Handling and Memory Instructions

| Mnemonic | Operands                  | Operation                          |
| -------- | ------------------------- | ---------------------------------- |
| `mov`    | `reg, reg/imm`            | `rd = rs/imm`                      |
| `movne`  | `reg, reg/imm`            | `if (cc & CC_NE) rd = rs/imm`      |
| `moveq`  | `reg, reg/imm`            | `if (cc & CC_EQ) rd = rs/imm`      |
| `movge`  | `reg, reg/imm`            | `if (cc & CC_GE) rd = rs/imm`      |
| `movgt`  | `reg, reg/imm`            | `if (cc & CC_GT) rd = rs/imm`      |
| `movle`  | `reg, reg/imm`            | `if (cc & CC_LE) rd = rs/imm`      |
| `movlt`  | `reg, reg/imm`            | `if (cc & CC_LT) rd = rs/imm`      |
| `movhs`  | `reg, reg/imm`            | `if (cc & CC_HS) rd = rs/imm`      |
| `movhi`  | `reg, reg/imm`            | `if (cc & CC_HI) rd = rs/imm`      |
| `movls`  | `reg, reg/imm`            | `if (cc & CC_LS) rd = rs/imm`      |
| `movlo`  | `reg, reg/imm`            | `if (cc & CC_LO) rd = rs/imm`      |
| `push`   | `reg`                     | `sp -= 4; *sp = rs`                |
| `pop`    | `reg`                     | `rd = *sp; sp += 4`                |
| `ldsb`   | `reg, (reg)/imm(reg)`     | `rd = sext(*(i8 *)rs/(imm + rs))`  |
| `ldsh`   | `reg, (reg)/imm(reg)`     | `rd = sext(*(i16 *)rs/(imm + rs))` |
| `ldub`   | `reg, (reg)/imm(reg)`     | `rd = zext(*(u8 *)rs/(imm + rs))`  |
| `lduh`   | `reg, (reg)/imm(reg)`     | `rd = zext(*(u16 *)rs/(imm + rs))` |
| `ldw`    | `reg, (reg)/imm(reg)`     | `rd = *rs/*(imm + rs)`             |
| `str`    | `reg/imm, (reg)/imm(reg)` | `*rd/*(imm + rd) = rs/imm`         |
| `lea`    | `reg, (label)/imm(label)` | `rd = &label/(&label + imm)`       |

#### Notes

- The first operand of `ldr` is the **destination** register (`rd`). Regardless
  of whether the second operand is a register or an immediate offset from a
  register, the resulting value is interpreted as the load *address*.
- The first operand of `str` is the **source** register (`rs`). Like `ldr`, it
  does not matter whether the second operand is a register or an immediate
  offset from a register, the resulting value is interpreted as the store
  *address*.

### String and I/O Instructions

| Mnemonic | Operands         | Operation                 |
| -------- | ---------------- | ------------------------- |
| `msg`    | `string, reg, *` | print arguments to stdout |
| `in`     | `(reg), reg/imm` | `read(0, (rd), rs/imm)`   |
| `out`    | `(reg), reg/imm` | `write(1, (rd), rs/imm)`  |

## Directives

`scisa` implements assembler directives for specifying segments and creating
initialized data.

| segment directive | description                |
| ----------------- | -------------------------- |
| `.text`           | mark start of text segment |
| `.data`           | mark start of data segment |

Use `.text` and `.data` to switch back and forth between segments as needed.

> Although usage of the data segment is optional, there is no default segment.
> If a program contains instructions, it must also contain a `.text` directive
> (and a halt instruction)

| data directive | syntax       | description                     | notes                     |
| -------------- | ------------ | ------------------------------- | ------------------------- |
| `.ascii`       | `.ascii str` | create a null-terminated string |                           |
| `.byte`        | `.byte imm`  | create a 1-byte value           | keeps lower byte of `imm` |
| `.hword`       | `.hword imm` | create a 2-byte value           | keeps lower half of `imm` |
| `.word`        | `.word imm`  | create a 4-byte value           |                           |
| `.zero`        | `.zero imm`  | emit `imm` zeros                |                           |
| `.align`       | `.align imm` | align data to `imm` bytes       |                           |

## Syntax

### Program Structure

```asm
.text           ; Start of code segment
main:           ; Entrypoint label

    (your instructions here)

    halt        ; End of entrypoint
```

### Comments

Comments start with a semicolon (`;`) and continue until the end of the line:

```asm
; This is a comment
mov r0, 42  ; This is an inline comment
```

---

Immediate values can be specified in decimal or hexadecimal format. Hexadecimal
values are prefixed with `0x` and are case-insensitive.

```asm
mov r4, 0x1000
mov r5, 0xFFFF
mov r6, 0xffff
```

Decimal values are specified without a prefix.

```asm
mov r5, 4096
```

Both decimal and hexadecimal values can be specified with a sign.

```asm
mov r6, -0x1000
mov r7, +0x1000 ; silly but valid
mov r8, -4096
mov r9, +4096   ; ditto
```

### ALU & Logic Instructions

Single register operand:

```asm
inc r4 ; r4 = r4 + 1
dec r5 ; r5 = r5 - 1
neg r6 ; r6 = 0 - r6
```

Register-register / Register-immediate operands:

```asm
add r1, r2      ; Add register r2 to r1
sub r3, 50      ; Subtract immediate value 50 from r3
mul r4, r5      ; Multiply r4 by r5
div r6, 2       ; Divide r6 by 2
```

### Stack Instructions

```asm
push r4 ; sp -= 4
        ; str r4, (sp)

pop r5  ; ldr r5, (sp)
        ; sp += 4
```

### Memory Instructions

Load and store instructions use a `reg, (reg)/imm(reg)` syntax.

```asm
ldr r18, 48(sp)
...
...
str r18, 0x30(sp)
```

```asm
add r18, 48
ldr r18, (sp)
...
...
add r18, 0x30
str r18, (sp)
```

---

The `lea` instruction uses a `reg, (label)/imm(label)` syntax.

```asm
.data
consts:
    .word   2146121005
    .word   -2073254261
    .word   1124208931
    .word   493478565

.text
hash:
    ...
    ...
    ; x *= consts[2]
    lea r2, 8(consts)
    ldr r3, r2
    mul r4, r3
    ...
    ...
    ; x *= consts[3]
    lea r2, 12(consts)
    ldr r3, r2
    mul r4, r3
    ...
    ...
```

Alternatively, keep the base label address on hand and apply an offset at load:

```asm
hash:
    lea r2, (consts)
    ...
    ...
    ; x *= consts[2]
    ldr r3, 8(r2)
    mul r4, r3
    ...
    ...
    ; x *= consts[3]
    ldr r3, 12(r2)
    mul r4, r3
    ...
    ...
```

---

### I/O Instructions

```asm
.text
main:
    ; Allocate buffer on the stack
    mov r0, sp       ; Save current stack pointer
    sub sp, 16       ; Allocate 16 bytes

    ; Read input from stdin (up to 16 bytes)
    mov r1, sp       ; Buffer address
    mov r2, 16       ; Buffer size
    in (r1), r2      ; Read from stdin

    ; Echo the input back to stdout
    out (r1), r2     ; Write to stdout

    ; Restore stack
    mov sp, r0
    halt
```

---

`msg` accepts a variable number of comma-separated arguments. Arguments can be
any combination of single-quoted strings and registers.

```asm
msg     'ackermann(', r4, ', ', r5, ') = ', r6, '\n'
```

`msg` buffers up to 64 bytes before flushing to stdout. A `msg` can be flushed
explicitly using a NUL byte.

```asm
msg     '\r', r4, '\t', r5, '\0'
```

### Control Flow Instructions

### Conditional Branch Instructions

```asm
.text
main:
    mov r1, 10      ; Initialize counter
    mov r2, 0       ; Initialize sum

loop:
    add r2, r1      ; Add counter to sum
    dec r1          ; Decrement counter

    cmp r1, 0       ; Compare counter with 0
    bgt loop        ; Branch to loop if counter > 0

    msg 'Sum: ', r2, '\n'  ; Print result
    halt
```

### CMP Instruction

`cmp` uses the following symantics to set the `cc` register:

```c
uint32_t setcc(uint32_t u0, uint32_t u1)
{
    int32_t i0 = u0;
    int32_t i1 = u1;
    uint32_t r = CC_NULL;
    r |= (u0 != u1) ? CC_NE : CC_NULL;
    r |= (u0 == u1) ? CC_EQ : CC_NULL;
    r |= (i0 >= i1) ? CC_GE : CC_NULL;
    r |= (i0  > i1) ? CC_GT : CC_NULL;
    r |= (i0 <= i1) ? CC_LE : CC_NULL;
    r |= (i0  < i1) ? CC_LT : CC_NULL;
    r |= (u0 >= u1) ? CC_HS : CC_NULL;
    r |= (u0  > u1) ? CC_HI : CC_NULL;
    r |= (u0 <= u1) ? CC_LS : CC_NULL;
    r |= (u0  < u1) ? CC_LO : CC_NULL;
    return r;
}
```

## TODO

```u
-fverbose-asm
-Os
-mips1
-mgp32
-msym32
-mlong32
-fno-delayed-branch
-mcompact-branches=never
-mno-micromips
-mno-mt
-mno-eva
-mno-mcu
-mno-virt
-mno-xpa
-mno-crc
-mno-mad
-mno-dsp
-mno-madd4
-mno-imadd
-mno-llsc
-mno-lxc1-sxc1
-mno-memcpy
-msoft-float
-mno-split-addresses
-mno-load-store-pairs
-mno-check-zero-division
-mframe-header-opt
```
