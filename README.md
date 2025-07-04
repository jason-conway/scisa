[<img align="center" src="./etc/scisa.png"/>](etc/scisa.png)

# `scisa`: Sweetened Condensed Instruction Set Architecture

## SCISA VM

General Purpose Registers
32 GPRs - r0 through r31

Special Purpose Registers
5 SPRs - pc, lr, sp, fp, cc
pc - program counter
lr - link register
sp - stack pointer
fp - frame pointer
cc - flags

2MB stack, (0x200000 bytes)

## The SCISA Instruction Set

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
| MOV      | reg, reg/imm     | r = r/imm                       |
| MSG      | string, reg, *   | print arguments to stdout       |
| INC      | reg              | r++                             |
| DEC      | reg              | r--                             |
| CMP      | reg/imm, reg/imm | CC = (d > s) - (d < s)          |
| JMP      | label            | PC = &label                     |
| JNE      | label            | if CC != 0 then PC = &label     |
| JE       | label            | if CC == 0 then PC = &label     |
| JGE      | label            | if CC >= 0 then PC = &label     |
| JG       | label            | if CC > 0 then PC = &label      |
| JLE      | label            | if CC <= 0 then PC = &label     |
| JL       | label            | if CC < 0 then PC = &label      |
| CALL     | label            | LR = PC; PC = &label            |
| RET      |                  | PC = LR                         |
| HLT      |                  | halt successfully               |

---

| Mnemonic | Operands     | Operation                                           |
| -------- | ------------ | --------------------------------------------------- |
| PUSH     | reg          | push a register value onto the stack                |
| POP      | reg          | pop a value from the stack into a register          |
| LDRB     | reg, reg/imm | load byte from memory into destination register     |
| LDRH     | reg, reg/imm | load halfword from memory into destination register |
| LDR      | reg, reg/imm | load word from memory into destination register     |
| STRB     | reg, reg/imm | store byte from source register into memory         |
| STRH     | reg, reg/imm | store halfword from source register into memory     |
| STR      | reg, reg/imm | store word from source register into memory         |

## Syntax

All mnemonics can be uppercase or lowercase: ADD == add

All registers can be uppercase or lowercase. R10 == r10

Registers names cann NOT be specified with hex. R31 == r31 but R31 != r1f

Immediate values can specified in decimal or hex.

A hexadecimal value must be prefixed with `0x`.

Intermediate values (both dec and hex) can be signed with `-` and `+`

Load and Store instructions support `reg, reg/imm/imm(reg)` syntax.

```asm
[ldr|str] r16, sp
[ldr|str] r16, 0x1000
[ldr|str] r16, 4096
[ldr|str] r16, 0x30(sp)
[ldr|str] r16, 48(sp)
```

---

## TODO

Let PUSH and POP accept a variable number of register operands
