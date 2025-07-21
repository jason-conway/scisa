| opcode | syntax                      | notes                               |
| ------ | --------------------------- | ----------------------------------- |
| incr   | inc <reg>                   |                                     |
| decr   | dec <reg>                   |                                     |
| negr   | neg <reg>                   |                                     |
| addri  | add <reg>, <imm32>          |                                     |
| addrr  | add <reg>, <reg>            |                                     |
| saddri | sadd <reg>, <imm32>         |                                     |
| saddrr | sadd <reg>, <reg>           |                                     |
| subri  | sub <reg>, <imm32>          |                                     |
| subrr  | sub <reg>, <reg>            |                                     |
| mulri  | mul <reg>, <imm32>          |                                     |
| mulrr  | mul <reg>, <reg>            |                                     |
| divri  | div <reg>, <imm32>          |                                     |
| divrr  | div <reg>, <reg>            |                                     |
| sdivri | sdiv <reg>, <imm32>         |                                     |
| sdivrr | sdiv <reg>, <reg>           |                                     |
| modri  | mod <reg>, <imm32>          |                                     |
| modrr  | mod <reg>, <reg>            |                                     |
| smodri | smod <reg>, <imm32>         |                                     |
| smodrr | smod <reg>, <reg>           |                                     |
| andri  | and <reg>, <imm32>          |                                     |
| andrr  | and <reg>, <reg>            |                                     |
| orri   | or <reg>, <imm32>           |                                     |
| orrr   | or <reg>, <reg>             |                                     |
| xorri  | xor <reg>, <imm32>          |                                     |
| xorrr  | xor <reg>, <reg>            |                                     |
| lslri  | lsl <reg>, <imm32>          |                                     |
| lslrr  | lsl <reg>, <reg>            |                                     |
| lsrri  | lsr <reg>, <imm32>          |                                     |
| lsrrr  | lsr <reg>, <reg>            |                                     |
| asrri  | asr <reg>, <imm32>          |                                     |
| asrrr  | asr <reg>, <reg>            |                                     |
| cmpii  | cmp <imm32>, <imm32>        |                                     |
| cmpir  | cmp <imm32>, <reg>          |                                     |
| cmpri  | cmp <reg>, <imm32>          |                                     |
| cmprr  | cmp <reg>, <reg>            |                                     |
| jmp    | jmp <label>                 |                                     |
| jne    | jne <label>                 |                                     |
| je     | je  <label>                 |                                     |
| jge    | jge <label>                 |                                     |
| jg     | jg  <label>                 |                                     |
| jle    | jle <label>                 |                                     |
| jl     | jle <label>                 |                                     |
| call   | call <label>                |                                     |
| movri  | mov <reg>, <imm32>          |                                     |
| movrr  | mov <reg>, <reg>            |                                     |
| pushr  | push <reg>                  |                                     |
| popr   | pop <reg>                   |                                     |
| ldrri  | ldr <reg>, <imm32>          |                                     |
| ldrrr  | ldr <reg>, <reg>            | value of src register is an address |
| ldrrir | ldr <reg>, <imm32>(<reg>)   |                                     |
| strii  | str <imm32>, <imm32>        | str <val> <addr>                    |
| strir  | str <imm32>, <reg>          |                                     |
| striir | str <imm32>, <imm32>(<reg>) |                                     |
| strri  | str <reg>, <imm32>          |                                     |
| strrr  | str <reg>, <reg>            |                                     |
| strrir | str <reg>, <imm32>(<reg>)   |                                     |
| learl  | lea <reg>, <label>          |                                     |
| learil | lea <reg>, <imm32>(<label>) |                                     |

| msg    |                      |
| halt   |                      |
| ret    |                      |
| abort  |                      |