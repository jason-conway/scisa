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

### Stack

The stack is 2MB in size, with the top of the stack at `0x7fffffff`.

| size | high address | low address |
| ---- | ------------ | ----------- |
| 2MB  | 0x7fffffff   | 0x7fdfffff  |

### Data Segment

The data segment is dynamically sized, with a base address of `0x10000000`.

## The SCISA Instruction Set

`scisa` implements a sweet (and condensed) set of instructions. The ISA intends
to be small without being constraining. Instructions are being added as needed.

### Arithmetic and Logic Instructions

| Mnemonic | Operands         | Operation                       |
| -------- | ---------------- | ------------------------------- |
| add      | reg, reg/imm     | r += (unsigned)r/imm            |
| sadd     | reg, reg/imm     | r += (signed)r/imm              |
| sub      | reg, reg/imm     | r -= (unsigned)r/imm            |
| mul      | reg, reg/imm     | r *= (unsigned)r/imm            |
| div      | reg, reg/imm     | r /= (unsigned)r/imm            |
| sdiv     | reg, reg/imm     | r /= (signed)r/imm              |
| mod      | reg, reg/imm     | r %= (unsigned)r/imm            |
| smod     | reg, reg/imm     | r %= (signed)r/imm              |
| and      | reg, reg/imm     | (unsigned)r &= (unsigned)r/imm  |
| or       | reg, reg/imm     | (unsigned)r \|= (unsigned)r/imm |
| xor      | reg, reg/imm     | (unsigned)r ^= (unsigned)r/imm  |
| lsl      | reg, reg/imm     | r <<= (unsigned)r/imm & mask    |
| lsr      | reg, reg/imm     | r >>= (unsigned)r/imm & mask    |
| asr      | reg, reg/imm     | (signed)r >>= r/imm & mask      |
| inc      | reg              | r++                             |
| dec      | reg              | r--                             |
| cmp      | reg/imm, reg/imm | cc = (d > s) - (d < s)          |

#### Notes

- Shift operations (`lsl`, `lsr`, `asr`) mask of `0x1f`
- The signed modulo instruction (`smod`) uses truncated division, matching the
  behavior of languages like C
- `cmp` *does* allow imm/imm comparison

### Control Flow Instructions

| Mnemonic | Operands | Operation               |
| -------- | -------- | ----------------------- |
| jmp      | label    | pc = &label             |
| jne      | label    | if (cc) pc = &label     |
| je       | label    | if (!cc) pc = &label    |
| jge      | label    | if (cc ≥ 0) pc = &label |
| jg       | label    | if (cc > 0) pc = &label |
| jle      | label    | if (cc ≤ 0) pc = &label |
| jl       | label    | if (cc < 0) pc = &label |
| call     | label    | lr = pc; pc = &label    |
| ret      |          | pc = lr                 |
| halt     |          | halt successfully       |

### Data Handling and Memory Instructions

| Mnemonic | Operands                  | Operation                                                                      |
| -------- | ------------------------- | ------------------------------------------------------------------------------ |
| mov      | reg, reg/imm              | r = r/imm                                                                      |
| push     | reg                       | push a register value onto the stack                                           |
| pop      | reg                       | pop a value from the stack into a register                                     |
| ldr      | reg, reg/imm/imm(reg)     | load word from memory into destination register                                |
| str      | reg/imm, reg/imm/imm(reg) | store word from source register into memory                                    |
| lea      | reg, label/imm(label)     | load the effective address of the source operand into the destination register |

#### Notes

- The first operand of `ldr` is the **destination** register. Regardless of whether
  the second operand is an immediate value, a register, or an immediate offset
  from a register, the value of the operand is interpreted as a memory address
  specifying where to load a word from.
- The first operand of `str` is the **source** register. Like `ldr`, it does not
  matter whether the second operand is an immediate value, a register, or an
  immediate offset from a register. The value of the operand is interpreted as a
  memory address specifying where to store the value of the source register to.

For example, the value

```asm
mov r8, 0x10000000

```

### String and Output Instructions

| Mnemonic | Operands       | Operation                 |
| -------- | -------------- | ------------------------- |
| msg      | string, reg, * | print arguments to stdout |

## Syntax

All mnemonics can be uppercase or lowercase:

```asm
add r1, r2
```

and

```asm
ADD r1, r2
```

are equivalent. Similarly, all registers can be uppercase or lowercase:

```asm
mul r1, r2
```

and

```asm
mul R1, R2
```

are equivalent.

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

---

### `op reg, reg/imm`

```asm
add r1, r2
sub r3, 50
```

### `op reg, reg/imm/imm(reg)`

```asm
mov r5, sp
ldr r4, r5 ; *r5 = r4
```

```asm
mov r5, 0x10000000
ldr r4, r5 ; r4 = *0x10000000
```

```asm
ldr r4, 0x10000000 ; r4 = *0x10000000
```

```asm
mov r5, 0x10000000
add r5, 16
ldr r4, r5
```

is identical to

```asm
mov r5, 0x10000000
ldr r4, 16(r5)
```

### `op reg`

#### ALU & Logic Instructions

```asm
inc r4 ; r4 = r4 + 1
dec r5 ; r5 = r5 - 1
```

#### Stack Instructions

```asm
push r4 ; sp -= 4
        ; str r4, sp

pop r5  ; ldr r5, sp
        ; sp += 4
```

### `cmp`

```asm
; r4 = 5
; r5 = 10
cmp r4, r5 ; cc = -1
cmp r4, 10 ; cc = -1
cmp 5, r5  ; cc = -1
cmp 5, 10  ; cc = -1
```

### Directives

`scisa` implements assembler directives for specifying segments and creating
initialized data.

| segment directive | description                |
| ----------------- | -------------------------- |
| .text             | mark start of text segment |
| .data             | mark start of data segment |

Use `.text` and `.data` to switch back and forth between segments as needed.

> Although usage of the data segment is optional, there is no default segment.
> If a program contains instructions, it must also contain a `.text` directive.

| data directive | syntax                         | description           |
| -------------- | ------------------------------ | --------------------- |
| .word          | `.word 0x2315`<br>`.word 8981` | create a 4-byte value |
| .byte          | `.byte 0x23`<br>`.byte 35`     | create a 1-byte value |

### Assembly Syntax

#### `reg, reg/imm`

Most instructions use the `reg, reg/imm` syntax.

```asm
add r1, r2
sub r3, 50
```

---

#### `reg, reg/imm/imm(reg)`

Load and store instructions support a `reg, reg/imm/imm(reg)` syntax.

```asm
ldr r18, 48(sp)
...
...
str r18, 0x30(sp)
```

---

#### `reg, label/imm(label)`

The `LEA` instruction supports `reg, label/imm(label)` syntax.

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
    lea r2, consts
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

#### `msg` Syntax

`msg` accepts a variable number of comma-seperated arguments. Arguments can be
any combination of single-quoted strings and registers.

```asm
msg     'ackermann(', r4, ', ', r5, ') = ', r6, '\n'
```

`msg` buffers up to 64 bytes before flushing to stdout. A `msg` can be flushed
explicitly using a NUL byte.

```asm
msg     '\r', r4, '\t', r5, '\0'
```

## TODO

Let PUSH and POP accept a variable number of register operands

```u
-Os
-mips32r5
-fno-delayed-branch
-mno-micromips
-mno-mt
-mno-eva
-mno-mcu
-mgp32
-mno-virt
-mno-xpa
-mno-crc
-msym32
-mlong32
-msoft-float
-mno-split-addresses
-fverbose-asm
-mno-load-store-pairs
-mno-llsc
-mno-dsp
-mno-check-zero-division
-mno-memcpy
-mno-mad
-mno-imadd
-mno-madd4
-mno-lxc1-sxc1
-mcompact-branches=never
-mframe-header-opt
```
