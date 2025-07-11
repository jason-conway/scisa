<!-- markdownlint-disable MD024 MD033 MD041 MD045 -->

# `scisa`: Sweetened Condensed Instruction Set Architecture

`scisa` is a losely defined ISA and an assembly interpreter for it.

---

[<img align="center" src="./etc/scisa.png"/>](etc/scisa.png)

---

## SCISA VM

### General Purpose Registers

`scisa` has 32 general purpose registers, `r0` through `r31`.

### Special Purpose Registers

`scisa` has 5 special purpose registers.

| reg | description     |
| --- | --------------- |
| pc  | program counter |
| lr  | link register   |
| sp  | stack pointer   |
| fp  | frame pointer   |
| cc  | flags           |

### Stack

The stack is 2MB in size, with the top of the stack at `0x7fffffff`.

| size | high address | low address |
| ---- | ------------ | ----------- |
| 2MB  | 0x7fffffff   | 0x7fdfffff  |

## The SCISA Instruction Set

`scisa` implements a sweet (and condensed) set of instructions.

### Arithmetic and Logic Instructions

| Mnemonic | Operands         | Operation                       |
| -------- | ---------------- | ------------------------------- |
| ADD      | reg, reg/imm     | r += (unsigned)r/imm            |
| SADD     | reg, reg/imm     | r += (signed)r/imm              |
| SUB      | reg, reg/imm     | r -= (unsigned)r/imm            |
| MUL      | reg, reg/imm     | r *= (unsigned)r/imm            |
| DIV      | reg, reg/imm     | r /= (unsigned)r/imm            |
| SDIV     | reg, reg/imm     | r /= (signed)r/imm              |
| MOD      | reg, reg/imm     | r %= (unsigned)r/imm            |
| SMOD     | reg, reg/imm     | r %= (signed)r/imm              |
| AND      | reg, reg/imm     | (unsigned)r &= (unsigned)r/imm  |
| OR       | reg, reg/imm     | (unsigned)r \|= (unsigned)r/imm |
| XOR      | reg, reg/imm     | (unsigned)r ^= (unsigned)r/imm  |
| LSL      | reg, reg/imm     | r <<= (unsigned)r/imm & mask    |
| LSR      | reg, reg/imm     | r >>= (unsigned)r/imm & mask    |
| ASR      | reg, reg/imm     | (signed)r >>= r/imm & mask      |
| INC      | reg              | r++                             |
| DEC      | reg              | r--                             |
| CMP      | reg/imm, reg/imm | CC = (d > s) - (d < s)          |

#### Notes

- Shift operations (`LSL`, `LSR`, `ASR`) mask of $\texttt{0x1f}$.
- The signed modulo instruction (`SMOD`) uses truncated division, matching the
  behavior of languages like C.
- `CMP` *does* allow imm/imm comparison.

### Control Flow Instructions

| Mnemonic | Operands | Operation                               |
| -------- | -------- | --------------------------------------- |
| JMP      | label    | PC = &label                             |
| JNE      | label    | if (CC)     {<br>&emsp;PC = &label<br>} |
| JE       | label    | if (!CC)    {<br>&emsp;PC = &label<br>} |
| JGE      | label    | if (CC ≥ 0) {<br>&emsp;PC = &label<br>} |
| JG       | label    | if (CC > 0) {<br>&emsp;PC = &label<br>} |
| JLE      | label    | if (CC ≤ 0) {<br>&emsp;PC = &label<br>} |
| JL       | label    | if (CC < 0) {<br>&emsp;PC = &label<br>} |
| CALL     | label    | LR = PC<br>PC = &label                  |
| RET      |          | PC = LR                                 |
| HALT     |          | halt successfully                       |

### Data Handling and Memory Instructions

| Mnemonic | Operands              | Operation                                                                      |
| -------- | --------------------- | ------------------------------------------------------------------------------ |
| MOV      | reg, reg/imm          | r = r/imm                                                                      |
| PUSH     | reg                   | push a register value onto the stack                                           |
| POP      | reg                   | pop a value from the stack into a register                                     |
| LDR      | reg, reg/imm/imm(reg) | load word from memory into destination register                                |
| STR      | reg, reg/imm/imm(reg) | store word from source register into memory                                    |
| LEA      | reg, label/imm(label) | load the effective address of the source operand into the destination register |

### String and Output Instructions

| Mnemonic | Operands       | Operation                 |
| -------- | -------------- | ------------------------- |
| MSG      | string, reg, * | print arguments to stdout |

## Syntax

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

#### Notes

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

#### Immediate Values

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

`-O1 -mframe-header-opt -fno-delayed-branch -mgp32 -mlong32 -mlong-calls -mno-split-addresses -fverbose-asm -mno-load-store-pairs`