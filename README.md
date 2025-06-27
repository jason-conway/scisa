[<img align="left" src="./etc/scisa-small.png" width="300"/>](etc/scisa-small.png)

# `scisa`: Sweetened Condensed Instruction Set Architecture

| Operands     | Mnemonic | Description                          | Opcode    |
| ------------ | -------- | ------------------------------------ | --------- |
| reg          | inc      | increment register                   | op_incr   |
| reg          | dec      | decrement register                   | op_decr   |
| reg          | neg      | signed negate                        | op_negr   |
| reg          | not      | bitwise complement                   | op_notr   |
| reg, imm     | mov      | move immediate to register           | op_movri  |
| reg, reg     | mov      | move register to register            | op_movrr  |
| reg, imm     | add      | add immediate to register (unsigned) | op_addri  |
| reg, reg     | add      | add register to register (unsigned)  | op_addrr  |
| reg, imm     | sadd     | add immediate to register (signed)   | op_saddri |
| reg, reg     | sadd     | add register to register (signed)    | op_saddrr |
| reg, imm     | sub      | subtruct immediate from register     | op_subri  |
| reg, reg     | sub      | subtruct register from register      | op_subrr  |
| reg, imm     | mul      | multiply immediate with register     | op_mulri  |
| reg, reg     | mul      | multiply register with register      | op_mulrr  |
| reg, imm     | div      | unsigned division                    | op_divri  |
| reg, reg     | div      | unsigned division                    | op_divrr  |
| reg, imm     | sdiv     | signed division                      | op_sdivri |
| reg, reg     | sdiv     | signed division                      | op_sdivrr |
| reg, imm     | mod      | unsigned modulo                      | op_modri  |
| reg, reg     | mod      | unsigned modulo                      | op_modrr  |
| reg, imm     | smod     | signed modulo                        | op_smodri |
| reg, reg     | smod     | signed modulo                        | op_smodrr |
| reg, imm     | and      | bitwise and                          | op_andri  |
| reg, reg     | and      | bitwise and                          | op_andrr  |
| reg, imm     | orr      | bitwise or                           | op_orri   |
| reg, reg     | orr      | bitwise or                           | op_orrr   |
| reg, imm     | xor      | bitwise xor                          | op_xorri  |
| reg, reg     | xor      | bitwise xor                          | op_xorrr  |
| reg, imm     | lsl      | logical shift left                   | op_lslri  |
| reg, reg     | lsl      | logical shift left                   | op_lslrr  |
| reg, imm     | lsr      | logical shift right                  | op_lsrri  |
| reg, reg     | lsr      | logical shift right                  | op_lsrrr  |
| reg, imm     | asr      | arithmetic shift right               | op_asrri  |
| reg, reg     | asr      | arithmetic shift right               | op_asrrr  |
| imm, reg     | cmp      | signed compare                       | op_cmpir  |
| reg, imm     | cmp      | signed compare                       | op_cmpri  |
| reg, reg     | cmp      | signed compare                       | op_cmprr  |
| label        | jmp      | jump unconditionally                 | op_jmp    |
| label        | jne      | branch not equal                     | op_jne    |
| label        | je       | branch if equal                      | op_je     |
| label        | jge      | branch if greater than or equal to   | op_jge    |
| label        | jg       | branch if greater than               | op_jg     |
| label        | jle      | branch if less than or equal to      | op_jle    |
| label        | jl       | branch if less than                  | op_jl     |
| label        | call     | call subroutine                      | op_call   |
|              | ret      | return from subroutine               | op_ret    |
| string, R, * | msg      | print operands                       | op_msg    |
|              | halt     | halt successfully                    | op_halt   |
