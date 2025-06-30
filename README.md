[<img align="left" src="./etc/scisa-small.png" width="300"/>](etc/scisa-small.png)

# `scisa`: Sweetened Condensed Instruction Set Architecture

## SCISA VM


## The SCISA Instruction Set

| Mnemonic | Operands         | Operation                      |
| -------- | ---------------- | ------------------------------ |
| ADD      | reg, reg/imm     | r += r/imm                     |
| SADD     | reg, reg/imm     | r += (signed)r/imm             |
| SUB      | reg, reg/imm     | r -= r/imm                     |
| MUL      | reg, reg/imm     | r *= r/imm                     |
| DIV      | reg, reg/imm     | r /= r/imm                     |
| SDIV     | reg, reg/imm     | r /= (signed)r/imm             |
| MOD      | reg, reg/imm     | r %= r/imm                     |
| SMOD     | reg, reg/imm     | r %= (signed)r/imm             |
| AND      | reg, reg/imm     | r &= r/imm                     |
| OR       | reg, reg/imm     | r \|= r/imm                    |
| XOR      | reg, reg/imm     | r ^= r/imm                     |
| LSL      | reg, reg/imm     | r <<= r/imm                    |
| LSR      | reg, reg/imm     | r >>= r/imm                    |
| ASR      | reg, reg/imm     | (signed)r >>= r/imm            |
| MOV      | reg, reg/imm     | r = r/imm                      |
| MSG      | string, reg, *   | print arguments to stdout      |
| INC      | reg              | r++                            |
| DEC      | reg              | r--                            |
| CMP      | reg/imm, reg/imm | FLAGS = (d > s) - (d < s)      |
| JMP      | label            | PC := label                    |
| JNE      | label            | if FLAGS != 0 then PC := label |
| JE       | label            | if FLAGS == 0 then PC := label |
| JGE      | label            | if FLAGS >= 0 then PC := label |
| JG       | label            | if FLAGS > 0 then PC := label  |
| JLE      | label            | if FLAGS <= 0 then PC := label |
| JL       | label            | if FLAGS < 0 then PC := label  |
| CALL     | label            | PUSH(PC); PC := label          |
| RET      |                  | PC := POP()                    |
| HLT      |                  | halt successfully              |


| Mnemonic | Operands     | Operation                                           |
| -------- | ------------ | --------------------------------------------------- |
| LDRB     | reg, reg/imm | load byte from memory into destination register     |
| LDRH     | reg, reg/imm | load halfword from memory into destination register |
| LDR      | reg, reg/imm | load word from memory into destination register     |
| STRB     | reg, reg/imm | store byte from source register into memory         |
| STRH     | reg, reg/imm | store halfword from source register into memory     |
| STR      | reg, reg/imm | store word from source register into memory         |


| Mnemonic | Operands     | Description                                  | Opcode    |
| -------- | ------------ | -------------------------------------------- | --------- |
| halt     |              |                                              | op_halt   |
| inc      | reg          | increment register                           | op_incr   |
| dec      | reg          | decrement register                           | op_decr   |
| neg      | reg          | signed negate                                | op_negr   |
| mov      | reg, imm     | move immediate to register                   | op_movri  |
| mov      | reg, reg     | move register to register                    | op_movrr  |
| add      | reg, imm     | add immediate to register (unsigned)         | op_addri  |
| add      | reg, reg     | add register to register (unsigned)          | op_addrr  |
| sadd     | reg, imm     | add immediate to register (signed)           | op_saddri |
| sadd     | reg, reg     | add register to register (signed)            | op_saddrr |
| sub      | reg, imm     | subtruct immediate from register             | op_subri  |
| sub      | reg, reg     | subtruct register from register              | op_subrr  |
| mul      | reg, imm     | multiply immediate with register             | op_mulri  |
| mul      | reg, reg     | multiply register with register              | op_mulrr  |
| div      | reg, imm     | divide register by immediate (unsigned)      | op_divri  |
| div      | reg, reg     | divide register by register (unsigned)       | op_divrr  |
| sdiv     | reg, imm     | divide register by immediate (signed)        | op_sdivri |
| sdiv     | reg, reg     | divide register by register (signed)         | op_sdivrr |
| mod      | reg, imm     | modulo register by immediate (unsigned)      | op_modri  |
| mod      | reg, reg     | modulo register by register (unsigned)       | op_modrr  |
| smod     | reg, imm     | modulo register by immediate (signed)        | op_smodri |
| smod     | reg, reg     | modulo register by immediate (signed)        | op_smodrr |
| and      | reg, imm     | bitwise AND register with immediate          | op_andri  |
| and      | reg, reg     | bitwise AND register with register           | op_andrr  |
| orr      | reg, imm     | bitwise OR register with immediate           | op_orri   |
| orr      | reg, reg     | bitwise OR register with register            | op_orrr   |
| xor      | reg, imm     | bitwise XOR register with immediate          | op_xorri  |
| xor      | reg, reg     | bitwise XOR register with register           | op_xorrr  |
| lsl      | reg, imm     | logical shift left register by immediate     | op_lslri  |
| lsl      | reg, reg     | logical shift left register by register      | op_lslrr  |
| lsr      | reg, imm     | logical shift right register by immediate    | op_lsrri  |
| lsr      | reg, reg     | logical shift right register by register     | op_lsrrr  |
| asr      | reg, imm     | arithmetic shift right register by immediate | op_asrri  |
| asr      | reg, reg     | arithmetic shift right register by register  | op_asrrr  |
| ldrb     | reg, imm     | load byte from `[imm]` into register         |           |
| ldrb     | reg, reg     | load byte from `[reg]` into register         |           |
| ldrh     | reg, imm     | load halfword from `[imm]` into register     |           |
| ldrh     | reg, reg     | load halfword from `[reg]` into register     |           |
| ldr      | reg, imm     | load word from `[imm]` into register         |           |
| ldr      | reg, reg     | load word from `[reg]` into register         |           |
| strb     | reg, imm     | store byte from register at `[imm]`          |           |
| strb     | reg, reg     | store byte from register at `[reg]`          |           |
| strh     | reg, imm     | store halfword from register at `[imm]`      |           |
| strh     | reg, reg     | store halfword from register at `[reg]`      |           |
| str      | reg, imm     | store word from register at `[imm]`          |           |
| str      | reg, reg     | store word from register at `[reg]`          |           |
| msg      | string, R, * | print operands                               | op_msg    |
| cmp      | imm, reg     | compare immediate with register (signed)     | op_cmpir  |
| cmp      | reg, imm     | compare register with immediate (signed)     | op_cmpri  |
| cmp      | reg, reg     | compare register with register (signed)      | op_cmprr  |
| jmp      | `label`      | jump to `label` unconditionally              | op_jmp    |
| jne      | `label`      | jump to `label` if not equal                 | op_jne    |
| je       | `label`      | jump to `label` if equal                     | op_je     |
| jge      | `label`      | jump to `label` if greater than or equal to  | op_jge    |
| jg       | `label`      | jump to `label` if greater than              | op_jg     |
| jle      | `label`      | jump to `label` if less than or equal to     | op_jle    |
| jl       | `label`      | jump to `label` if less than                 | op_jl     |
| call     | `label`      | call subroutine                              | op_call   |
| ret      |              | return from subroutine                       | op_ret    |


## Notes
