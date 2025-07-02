[<img align="left" src="./etc/scisa-small.png" width="300"/>](etc/scisa-small.png)

# `scisa`: Sweetened Condensed Instruction Set Architecture

## SCISA VM

32 GPRs         [r0 through r31]
program counter [pc]
link register   [lr]
stack pointer   [sp]
frame pointer   [fp]
flags           [cc]

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
| PUSH     | reg          | push a register onto the stack                      |
| POP      | reg          | pop a value from the stack into the register        |
| LDRB     | reg, reg/imm | load byte from memory into destination register     |
| LDRH     | reg, reg/imm | load halfword from memory into destination register |
| LDR      | reg, reg/imm | load word from memory into destination register     |
| STRB     | reg, reg/imm | store byte from source register into memory         |
| STRH     | reg, reg/imm | store halfword from source register into memory     |
| STR      | reg, reg/imm | store word from source register into memory         |

---

## TODO

Let PUSH and POP accept a variable number of register operands
