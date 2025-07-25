# Opcodes

Instruction types:

- `R` - register-register
- `I` - ALU immediate instructions
- `J` - operand is a label
- `U` - register

| opcode    | syntax                  | notes                                                                                                     |
| --------- | ----------------------- | --------------------------------------------------------------------------------------------------------- |
| `incr`    | `inc    rd`             | reads and writes to `rd`                                                                                  |
| `decr`    | `dec    rd`             | reads and writes to `rd`                                                                                  |
| `negr`    | `neg    rd`             | reads and writes to `rd`                                                                                  |
| `movri`   | `mov    rd, imm`        | `imm` is interpreted as a 32-bit unsigned value                                                           |
| `movrr`   | `mov    rd, rs`         |                                                                                                           |
| `addri`   | `add    rd, imm`        | `imm` is interpreted as a 32-bit unsigned value                                                           |
| `addrr`   | `add    rd, rs`         |                                                                                                           |
| `saddri`  | `sadd   rd, imm`        | `imm` is interpreted as a 32-bit signed value                                                             |
| `saddrr`  | `sadd   rd, rs`         |                                                                                                           |
| `subri`   | `sub    rd, imm`        | `imm` is interpreted as a 32-bit unsigned value                                                           |
| `subrr`   | `sub    rd, rs`         |                                                                                                           |
| `mulri`   | `mul    rd, imm`        | `imm` is interpreted as a 32-bit unsigned value                                                           |
| `mulrr`   | `mul    rd, rs`         |                                                                                                           |
| `divri`   | `div    rd, imm`        | `imm` is interpreted as a 32-bit unsigned value                                                           |
| `divrr`   | `div    rd, rs`         |                                                                                                           |
| `sdivri`  | `sdiv   rd, imm`        | `imm` is interpreted as a 32-bit signed value                                                             |
| `sdivrr`  | `sdiv   rd, rs`         |                                                                                                           |
| `modri`   | `mod    rd, imm`        | `imm` is interpreted as a 32-bit unsigned value                                                           |
| `modrr`   | `mod    rd, rs`         |                                                                                                           |
| `smodri`  | `smod   rd, imm`        | `imm` is interpreted as a 32-bit signed value                                                             |
| `smodrr`  | `smod   rd, rs`         |                                                                                                           |
| `andri`   | `and    rd, imm`        | `imm` is interpreted as a 32-bit unsigned value                                                           |
| `andrr`   | `and    rd, rs`         |                                                                                                           |
| `orri`    | `or     rd, imm`        | `imm` is interpreted as a 32-bit unsigned value                                                           |
| `orrr`    | `or     rd, rs`         |                                                                                                           |
| `xorri`   | `xor    rd, imm`        | `imm` is interpreted as a 32-bit unsigned value                                                           |
| `xorrr`   | `xor    rd, rs`         |                                                                                                           |
| `lslri`   | `lsl    rd, imm`        | `imm` is interpreted as a 32-bit unsigned value                                                           |
| `lslrr`   | `lsl    rd, rs`         |                                                                                                           |
| `lsrri`   | `lsr    rd, imm`        | `imm` is interpreted as a 32-bit unsigned value                                                           |
| `lsrrr`   | `lsr    rd, rs`         |                                                                                                           |
| `asrri`   | `asr    rd, imm`        | `imm` is interpreted as a 32-bit unsigned value                                                           |
| `asrrr`   | `asr    rd, rs`         |                                                                                                           |
| `movneri` | `movne  rd, imm`        | reads `cc` register                                                                                       |
| `movnerr` | `movne  rd, rs`         | reads `cc` register                                                                                       |
| `moveqri` | `moveq  rd, imm`        | reads `cc` register                                                                                       |
| `moveqrr` | `moveq  rd, rs`         | reads `cc` register                                                                                       |
| `movgeri` | `movge  rd, imm`        | reads `cc` register                                                                                       |
| `movgerr` | `movge  rd, rs`         | reads `cc` register                                                                                       |
| `movgtri` | `movgt  rd, imm`        | reads `cc` register                                                                                       |
| `movgtrr` | `movgt  rd, rs`         | reads `cc` register                                                                                       |
| `movleri` | `movle  rd, imm`        | reads `cc` register                                                                                       |
| `movlerr` | `movle  rd, rs`         | reads `cc` register                                                                                       |
| `movltri` | `movlt  rd, imm`        | reads `cc` register                                                                                       |
| `movltrr` | `movlt  rd, rs`         | reads `cc` register                                                                                       |
| `cmpii`   | `cmp    imm1, imm2`     | `imm1` and `imm2` interpreted as a 32-bit signed value, sets `cc` register                                |
| `cmpir`   | `cmp    imm, rs`        | `imm` is interpreted as a 32-bit signed value, sets `cc` register                                         |
| `cmpri`   | `cmp    rd, imm`        | `imm` is interpreted as a 32-bit signed value, sets `cc` register                                         |
| `cmprr`   | `cmp    rd, rs`         | `imm` is interpreted as a 32-bit signed value, sets `cc` register                                         |
| `jmp`     | `jmp    label`          | reads `cc` register                                                                                       |
| `jne`     | `jne    label`          | reads `cc` register                                                                                       |
| `jeq`     | `je     label`          | reads `cc` register                                                                                       |
| `jge`     | `jge    label`          | reads `cc` register                                                                                       |
| `jgt`     | `jg     label`          | reads `cc` register                                                                                       |
| `jle`     | `jle    label`          | reads `cc` register                                                                                       |
| `jlt`     | `jle    label`          | reads `cc` register                                                                                       |
| `call`    | `call   label`          | writes `lr` register, reads and writes `pc`                                                               |
| `ret`     |                         | reads `lr` register, writes `pc` register                                                                 |
| `pushr`   | `push   rs`             | reads and writes `sp` register                                                                            |
| `popr`    | `pop    rd`             | reads and writes `sp` register                                                                            |
| `ldrri`   | `ldr    rd, imm`        | `imm` treated as memory address                                                                           |
| `ldrrr`   | `ldr    rd, rs`         | treats value in `rs` as memory address                                                                    |
| `ldrrir`  | `ldr    rd, imm(rs)`    | treats value in `rs` as address, offsets by `imm`, `imm` is interpreted as a 32-bit signed value          |
| `strii`   | `str    imm1, imm2`     | `imm2` treated as memory address                                                                          |
| `strir`   | `str    imm, rd`        | treats value in `rd` as memory address                                                                    |
| `striir`  | `str    imm1, imm2(rd)` | treats value in `rd` as memory address, offsets by `imm2`, `imm2` is interpreted as a 32-bit signed value |
| `strri`   | `str    rs, imm`        | `imm` treated as memory address                                                                           |
| `strrr`   | `str    rs, rd`         | treats value in `rd` as memory address                                                                    |
| `strrir`  | `str    rs, imm(rd)`    | treats value in `rd` as memory address, offsets by `imm`, `imm` is interpreted as a 32-bit signed value   |
| `learl`   | `lea    rd, label`      |                                                                                                           |
| `learil`  | `lea    rd, imm(label)` | `imm` is interpreted as a 32-bit signed value                                                             |

| msg    |                      |
| halt   |                      |
| abort  |                      |