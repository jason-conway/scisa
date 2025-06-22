[<img align="left" src="./etc/scisa-small.png" width="300"/>](etc/scisa-small.png)

# `scisa`: Sweetened Condensed Instruction Set Architecture


| Opcode    | Operands     | Mnemonic | Description                          |
| --------- | ------------ | -------- | ------------------------------------ |
| op_incr   | reg          | inc      | increment register                   |
| op_decr   | reg          | dec      | decrement register                   |
| op_negr   | reg          | neg      | signed negate                        |
| op_notr   | reg          | not      | bitwise complement                   |
| op_movri  | reg, imm     | mov      | move immediate to register           |
| op_movrr  | reg, reg     | mov      | move register to register            |
| op_addri  | reg, imm     | add      | add immediate to register (unsigned) |
| op_addrr  | reg, reg     | add      | add register to register (unsigned)  |
| op_saddri | reg, imm     | sadd     | add immediate to register (signed)   |
| op_saddrr | reg, reg     | sadd     | add register to register (signed)    |
| op_subri  | reg, imm     | sub      | subtruct immediate from register     |
| op_subrr  | reg, reg     | sub      | subtruct register from register      |
| op_mulri  | reg, imm     | mul      | multiply immediate with register     |
| op_mulrr  | reg, reg     | mul      | multiply register with register      |
| op_divri  | reg, imm     | div      | unsigned division                    |
| op_divrr  | reg, reg     | div      | unsigned division                    |
| op_sdivri | reg, imm     | sdiv     | signed division                      |
| op_sdivrr | reg, reg     | sdiv     | signed division                      |
| op_modri  | reg, imm     | mod      | unsigned modulo                      |
| op_modrr  | reg, reg     | mod      | unsigned modulo                      |
| op_smodri | reg, imm     | smod     | signed modulo                        |
| op_smodrr | reg, reg     | smod     | signed modulo                        |
| op_andri  | reg, imm     | and      | bitwise and                          |
| op_andrr  | reg, reg     | and      | bitwise and                          |
| op_orri   | reg, imm     | orr      | bitwise or                           |
| op_orrr   | reg, reg     | orr      | bitwise or                           |
| op_xorri  | reg, imm     | xor      | bitwise xor                          |
| op_xorrr  | reg, reg     | xor      | bitwise xor                          |
| op_lslri  | reg, imm     | lsl      | logical shift left                   |
| op_lslrr  | reg, reg     | lsl      | logical shift left                   |
| op_lsrri  | reg, imm     | lsr      | logical shift right                  |
| op_lsrrr  | reg, reg     | lsr      | logical shift right                  |
| op_asrri  | reg, imm     | asr      | arithmetic shift right               |
| op_asrrr  | reg, reg     | asr      | arithmetic shift right               |
| op_cmpir  | imm, reg     | cmp      | signed compare                       |
| op_cmpri  | reg, imm     | cmp      | signed compare                       |
| op_cmprr  | reg, reg     | cmp      | signed compare                       |
| op_jmp    | label        | jmp      | jump unconditionally                 |
| op_jne    | label        | jne      | branch not equal                     |
| op_je     | label        | je       | branch if equal                      |
| op_jge    | label        | jge      | branch if greater than or equal to   |
| op_jg     | label        | jg       | branch if greater than               |
| op_jle    | label        | jle      | branch if less than or equal to      |
| op_jl     | label        | jl       | branch if less than                  |
| op_call   | label        | call     | call subroutine                      |
| op_ret    |              | ret      | return from subroutine               |
| op_msg    | string, R, * | msg      | print operands                       |
| op_halt   |              | halt     | halt successfully                    |
