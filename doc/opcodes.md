# Opcodes

| opcode    | syntax                  | notes                                                                          |
| --------- | ----------------------- | ------------------------------------------------------------------------------ |
| `incr`    | `inc    rd`             | `rd += 1`                                                                      |
| `decr`    | `dec    rd`             | `rd -= 1`                                                                      |
| `negr`    | `neg    rd`             | `rd = -rd`                                                                     |
| `movri`   | `mov    rd, imm`        | `rd = imm`                                                                     |
| `movrr`   | `mov    rd, rs`         | `rd = rs`                                                                      |
| `addri`   | `add    rd, imm`        | `rd += imm`                                                                    |
| `addrr`   | `add    rd, rs`         | `rd += rs`                                                                     |
| `subri`   | `sub    rd, imm`        | `rd -= imm`                                                                    |
| `subrr`   | `sub    rd, rs`         | `rd -= rs`                                                                     |
| `mulri`   | `mul    rd, imm`        | `rd *= imm`                                                                    |
| `mulrr`   | `mul    rd, rs`         | `rd *= rs`                                                                     |
| `divri`   | `div    rd, imm`        | `rd = (imm) ? (rd / imm) : 0`                                                  |
| `divrr`   | `div    rd, rs`         | `rd = (rs) ? (rd / rs) : 0`                                                    |
| `sdivri`  | `sdiv   rd, imm`        | `rd = (!imm) ? 0 : ((imm == -1 && rd == INT32_MIN) ? INT32_MIN : (rd s/ imm))` |
| `sdivrr`  | `sdiv   rd, rs`         | `rd = (!rs) ? 0 : ((rs == -1 && rd == INT32_MIN) ? INT32_MIN : (rd s/ rs))`    |
| `modri`   | `mod    rd, imm`        | `rd = (imm) ? (rs % imm) : rd`                                                 |
| `modrr`   | `mod    rd, rs`         | `rd = (rs) ? (rs % rs) : rd`                                                   |
| `smodri`  | `smod   rd, imm`        | `rd = (!imm) ? rd : ((imm == -1 && rd == INT32_MIN) ? 0: (rd s% imm))`         |
| `smodrr`  | `smod   rd, rs`         | `rd = (!rs) ? rd : ((rs == -1 && rd == INT32_MIN) ? 0: (rd s% rs))`            |
| `andri`   | `and    rd, imm`        | `rd = (unsigned)rd AND (unsigned)imm`                                          |
| `andrr`   | `and    rd, rs`         | `rd = (unsigned)rd AND (unsigned)rs`                                           |
| `orri`    | `or     rd, imm`        | `rd = (unsigned)rd OR (unsigned)imm`                                           |
| `orrr`    | `or     rd, rs`         | `rd = (unsigned)rd OR (unsigned)rs`                                            |
| `xorri`   | `xor    rd, imm`        | `rd = (unsigned)rd XOR (unsigned)imm`                                          |
| `xorrr`   | `xor    rd, rs`         | `rd = (unsigned)rd XOR (unsigned)rs`                                           |
| `lslri`   | `lsl    rd, imm`        | `rd <<= (imm & 0x1f)`                                                          |
| `lslrr`   | `lsl    rd, rs`         | `rd <<= (rs & 0x1f)`                                                           |
| `lsrri`   | `lsr    rd, imm`        | `rd >>= (imm & 0x1f)`                                                          |
| `lsrrr`   | `lsr    rd, rs`         | `rd >>= (rs & 0x1f)`                                                           |
| `asrri`   | `asr    rd, imm`        | `rd s>>= (imm & 0x1f)`                                                         |
| `asrrr`   | `asr    rd, rs`         | `rd s>>= (rs & 0x1f)`                                                          |
| `movneri` | `movne  rd, imm`        | `if (cc) rd = imm`                                                             |
| `movnerr` | `movne  rd, rs`         | `if (cc) rd = rs`                                                              |
| `moveqri` | `moveq  rd, imm`        | `if (!cc) rd = imm`                                                            |
| `moveqrr` | `moveq  rd, rs`         | `if (!cc) rd = rs`                                                             |
| `movgeri` | `movge  rd, imm`        | `if (cc ≥ 0) rd = imm`                                                         |
| `movgerr` | `movge  rd, rs`         | `if (cc ≥ 0) rd = rs`                                                          |
| `movgtri` | `movgt  rd, imm`        | `if (cc > 0) rd = imm`                                                         |
| `movgtrr` | `movgt  rd, rs`         | `if (cc > 0) rd = rs`                                                          |
| `movleri` | `movle  rd, imm`        | `if (cc ≤ 0) rd = imm`                                                         |
| `movlerr` | `movle  rd, rs`         | `if (cc ≤ 0) rd = rs`                                                          |
| `movltri` | `movlt  rd, imm`        | `if (cc < 0) rd = imm`                                                         |
| `movltrr` | `movlt  rd, rs`         | `if (cc < 0) rd = rs`                                                          |
| `cmpii`   | `cmp    imm1, imm2`     | `cc = (imm1 > imm2) - (imm1 < imm2)`                                           |
| `cmpir`   | `cmp    imm, rs`        | `cc = (imm > rs) - (imm < rs)`                                                 |
| `cmpri`   | `cmp    rd, imm`        | `cc = (rd > imm) - (rd < imm)`                                                 |
| `cmprr`   | `cmp    rd, rs`         | `cc = (rd > rs) - (rd < rs)`                                                   |
| `jmp`     | `jmp    label`          | `pc = &label`                                                                  |
| `jne`     | `jne    label`          | `if (cc) pc = &label`                                                          |
| `jeq`     | `je     label`          | `if (!cc) pc = &label`                                                         |
| `jge`     | `jge    label`          | `if (cc ≥ 0) pc = &label`                                                      |
| `jgt`     | `jg     label`          | `if (cc > 0) pc = &label`                                                      |
| `jle`     | `jle    label`          | `if (cc ≤ 0) pc = &label`                                                      |
| `jlt`     | `jle    label`          | `if (cc < 0) pc = &label`                                                      |
| `call`    | `call   label`          | `lr = pc; pc = &label`                                                         |
| `pushr`   | `push   rs`             | `sp -= 4; *sp = rs`                                                            |
| `popr`    | `pop    rd`             | `rd = *sp; sp += 4`                                                            |
| `ldrri`   | `ldr    rd, imm`        | `rd = *imm`                                                                    |
| `ldrrr`   | `ldr    rd, rs`         | `rd = *rs`                                                                     |
| `ldrrir`  | `ldr    rd, imm(rs)`    | `rd = *(rs + imm)`                                                             |
| `strii`   | `str    imm1, imm2`     | `*imm2 = imm1`                                                                 |
| `strir`   | `str    imm, rd`        | `*rd = imm`                                                                    |
| `striir`  | `str    imm1, imm2(rd)` | `*(rd + imm2) = imm1`                                                          |
| `strri`   | `str    rs, imm`        | `*imm = rs`                                                                    |
| `strrr`   | `str    rs, rd`         | `*rd = rs`                                                                     |
| `strrir`  | `str    rs, imm(rd)`    | `*(rd + imm) = rs`                                                             |
| `learl`   | `lea    rd, label`      | `rd = &label`                                                                  |
| `learil`  | `lea    rd, imm(label)` | `rd = &label + imm`                                                            |
| `msg`     |                         |                                                                                |
| `halt`    |                         |                                                                                |
| `abort`   |                         |                                                                                |
| `ret`     | `ret`                   | `pc = lr`                                                                      |