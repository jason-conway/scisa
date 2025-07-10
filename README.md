<!-- markdownlint-disable MD033 MD041 MD045 -->

# `scisa`: Sweetened Condensed Instruction Set Architecture

`scisa` is a losely defined ISA / and an assembly interpreter for it.

---

[<img align="center" src="./etc/scisa.png"/>](etc/scisa.png)

---

## SCISA VM

### General Purpose Registers

`scisa` has 32 general purpose registers, `r0` through `r31`.

### Special Purpose Registers

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

`scisa` has a sweet (and condensed) set of instruction

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
| LSL      | reg, reg/imm     | r <<= (unsigned)r/imm           |
| LSR      | reg, reg/imm     | r >>= (unsigned)r/imm           |
| ASR      | reg, reg/imm     | (signed)r >>= r/imm             |
| INC      | reg              | r++                             |
| DEC      | reg              | r--                             |
| CMP      | reg/imm, reg/imm | CC = (d > s) - (d < s)          |

### Control Flow Instructions

| Mnemonic | Operands         | Operation                   |
| -------- | ---------------- | --------------------------- |
| CMP      | reg/imm, reg/imm | CC = (d > s) - (d < s)      |
| JMP      | label            | PC = &label                 |
| JNE      | label            | if CC != 0 then PC = &label |
| JE       | label            | if CC == 0 then PC = &label |
| JGE      | label            | if CC >= 0 then PC = &label |
| JG       | label            | if CC > 0 then PC = &label  |
| JLE      | label            | if CC <= 0 then PC = &label |
| JL       | label            | if CC < 0 then PC = &label  |
| CALL     | label            | LR = PC; PC = &label        |
| RET      |                  | PC = LR                     |
| HALT     |                  | halt successfully           |

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

All mnemonics can be uppercase or lowercase: ADD == add

All registers can be uppercase or lowercase. R10 == r10

Registers names can NOT be specified with hex. R31 == r31 but R31 != r1f

#### Immediate Values

Immediate values can be specified in decimal or hexadecimal format.

Hexadecimal values are prefixed with `0x`

```asm
mov r4, 0x1000
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

#### `reg, reg/imm/imm(reg)`

Load and Store instructions support `reg, reg/imm/imm(reg)` syntax.

```asm
[ldr|str] r16, sp
[ldr|str] r16, 0x1000
[ldr|str] r16, 48(sp)
[ldr|str] r16, 0x30(sp)
```

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