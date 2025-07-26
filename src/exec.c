#include "scisa.h"

enum cfg { 
    STACK_HIGH = 0x7fffffff,
    STACK_SIZE = 1u << 21,
    STACK_LOW = STACK_HIGH - STACK_SIZE,
    DATA_LOW = 0x10000000,
};

static void *map_addr(scoff_t *obj, vaddr addr)
{
    if (likely((vaddr)(addr - STACK_LOW) < obj->stack.size)) {
        return &obj->stack.base[addr - STACK_LOW];
    }
    if (likely((vaddr)(addr - DATA_LOW) < obj->data.size)) {
        return &obj->data.base[addr - DATA_LOW];
    }
    __builtin_trap();
}

result_t execute(scoff_t obj, arena_t arena)
{
    result_t r = { 0 };
    r.out.fd = STDOUT_FILENO;
    r.out.cap = 1 << 6;
    r.out.data = alloc(&arena, uint8_t, r.out.cap);

    obj.stack.base = alloc(&arena, uint8_t, STACK_SIZE);
    obj.stack.size = STACK_SIZE;


    int32_t regs[r_max] = { 0 };
    regs[fp] = STACK_HIGH;
    regs[sp] = STACK_HIGH;
    regs[lr] = -1;
    regs[cc] = 0;
    regs[pc] = 0x0;

    scir_t *insns = obj.code;
    for (;; regs[pc]++) {
        int32_t a = 0;
        int32_t b = 0;
        int32_t rel = 0;
        scir_t *w = &insns[regs[pc]];
        switch (w->op) {
#pragma region ABORT
            case op_abort:
                return r;
#pragma endregion
#pragma region INC
            case op_incr:
                regs[w->reg[0]] += (uint32_t)1;
                break;
#pragma endregion
#pragma region DEC
            case op_decr:
                regs[w->reg[0]] -= (uint32_t)1;
                break;
#pragma endregion
#pragma region NEG
            case op_negr:
                regs[w->reg[0]] = -regs[w->reg[0]];
                break;
#pragma endregion
#pragma region PUSH
            case op_pushr:
                regs[sp] -= sizeof(uint32_t);
                __builtin_memcpy(map_addr(&obj, regs[sp]), &regs[w->reg[0]], sizeof(uint32_t));
                break;
#pragma endregion
#pragma region POP
            case op_popr:
                __builtin_memcpy(&regs[w->reg[0]], map_addr(&obj, regs[sp]), sizeof(uint32_t));
                regs[sp] += sizeof(uint32_t);
                break;
#pragma endregion
#pragma region MOV
            case op_movri:
                regs[w->reg[0]] = w->operand.imm[1];
                break;
            case op_movrr:
                regs[w->reg[0]] = regs[w->reg[1]];
                break;
#pragma endregion
#pragma region ADD
            case op_addri:
                regs[w->reg[0]] += (uint32_t)w->operand.imm[1];
                break;
            case op_addrr:
                regs[w->reg[0]] += (uint32_t)regs[w->reg[1]];
                break;
#pragma endregion
#pragma region SADD
            case op_saddri:
                regs[w->reg[0]] += w->operand.imm[1];
                break;
            case op_saddrr:
                regs[w->reg[0]] += regs[w->reg[1]];
                break;
#pragma endregion
#pragma region SUB
            case op_subri:
                regs[w->reg[0]] -= (uint32_t)w->operand.imm[1];
                break;
            case op_subrr:
                regs[w->reg[0]] -= (uint32_t)regs[w->reg[1]];
                break;
#pragma endregion
#pragma region MUL
            case op_mulri:
                regs[w->reg[0]] *= (uint32_t)w->operand.imm[1];
                break;
            case op_mulrr:
                regs[w->reg[0]] *= (uint32_t)regs[w->reg[1]];
                break;
#pragma endregion
#pragma region DIV
            case op_divri:
                switch (w->operand.imm[1]) {
                    case 0:
                        regs[w->reg[0]] = 0;
                    case -1:
                        regs[w->reg[0]] = -(uint32_t)regs[w->reg[0]];
                        break;
                    default:
                        regs[w->reg[0]] /= (uint32_t)w->operand.imm[1];
                }
                break;
            case op_divrr:
                switch (regs[w->reg[1]]) {
                    case 0:
                        regs[w->reg[0]] = 0;
                    case -1:
                        regs[w->reg[0]] = -(uint32_t)regs[w->reg[0]];
                        break;
                    default:
                        regs[w->reg[0]] /= (uint32_t)regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region SDIV
            case op_sdivri:
                switch (w->operand.imm[1]) {
                    case 0:
                        return r;
                    case -1:
                        regs[w->reg[0]] = -(uint32_t)regs[w->reg[0]];
                        break;
                    default:
                        regs[w->reg[0]] /= w->operand.imm[1];
                }
                break;
            case op_sdivrr:
                switch (regs[w->reg[1]]) {
                    case 0:
                        return r;
                    case -1:
                        regs[w->reg[0]] = -(uint32_t)regs[w->reg[0]];
                        break;
                    default:
                        regs[w->reg[0]] /= regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region MOD
            case op_modri:
                switch (w->operand.imm[1]) {
                    case 0:
                        break;
                    default:
                        regs[w->reg[0]] %= (uint32_t)w->operand.imm[1];
                }
                break;
            case op_modrr:
                switch (regs[w->reg[1]]) {
                    case 0:
                        break;
                    default:
                        regs[w->reg[0]] %= (uint32_t)regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region SMOD
            case op_smodri:
                switch (w->operand.imm[1]) {
                    case 0:
                        break;
                    case -1:
                        if (regs[w->reg[0]] == INT32_MIN) {
                            regs[w->reg[0]] = 0;
                            break;
                        }
                        // fallthrough
                    default:
                        regs[w->reg[0]] %= w->operand.imm[1];
                }
                break;
            case op_smodrr:
                switch (regs[w->reg[1]]) {
                    case 0:
                        break;
                    case -1:
                        if (regs[w->reg[0]] == INT32_MIN) {
                            regs[w->reg[0]] = 0;
                            break;
                        }
                        // fallthrough
                    default:
                        regs[w->reg[0]] %= regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region AND
            case op_andri:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] & (uint32_t)w->operand.imm[1];
                break;
            case op_andrr:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] & (uint32_t)regs[w->reg[1]];
                break;
#pragma endregion
#pragma region OR
            case op_orri:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] | (uint32_t)w->operand.imm[1];
                break;
            case op_orrr:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] | (uint32_t)regs[w->reg[1]];
                break;
#pragma endregion
#pragma region XOR
            case op_xorri:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] ^ (uint32_t)w->operand.imm[1];
                break;

            case op_xorrr:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] ^ (uint32_t)regs[w->reg[1]];
                break;
#pragma endregion
#pragma region LSL
            case op_lslri:
                regs[w->reg[0]] <<= ((uint32_t)w->operand.imm[1] & 0x1f);
                break;
            case op_lslrr:
                regs[w->reg[0]] <<= ((uint32_t)regs[w->reg[1]] & 0x1f);
                break;
#pragma endregion
#pragma region LSR
            case op_lsrri:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] >> ((uint32_t)w->operand.imm[1] & 0x1f);
                break;
            case op_lsrrr:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] >> ((uint32_t)regs[w->reg[1]] & 0x1f);
                break;
#pragma endregion
#pragma region ASR
            case op_asrri:
                regs[w->reg[0]] >>= ((uint32_t)w->operand.imm[1] & 0x1f);
                break;
            case op_asrrr:
                regs[w->reg[0]] >>= ((uint32_t)regs[w->reg[1]] & 0x1f);
                break;
#pragma endregion
#pragma region MOVNE
            case op_movneri:
                if (regs[cc]) {
                    regs[w->reg[0]] = w->operand.imm[1];
                }
                break;
            case op_movnerr:
                if (regs[cc]) {
                    regs[w->reg[0]] = regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region MOVEQ
            case op_moveqri:
                if (!regs[cc]) {
                    regs[w->reg[0]] = w->operand.imm[1];
                }
                break;
            case op_moveqrr:
                if (!regs[cc]) {
                    regs[w->reg[0]] = regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region MOVGE
            case op_movgeri:
                if (regs[cc] >= 0) {
                    regs[w->reg[0]] = w->operand.imm[1];
                }
                break;
            case op_movgerr:
                if (regs[cc] >= 0) {
                    regs[w->reg[0]] = regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region MOVGT
            case op_movgtri:
                if (regs[cc] > 0) {
                    regs[w->reg[0]] = w->operand.imm[1];
                }
                break;
            case op_movgtrr:
                if (regs[cc] > 0) {
                    regs[w->reg[0]] = regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region MOVLE
            case op_movleri:
                if (regs[cc] <= 0) {
                    regs[w->reg[0]] = w->operand.imm[1];
                }
                break;
            case op_movlerr:
                if (regs[cc] <= 0) {
                    regs[w->reg[0]] = regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region MOVLT
            case op_movltri:
                if (regs[cc] < 0) {
                    regs[w->reg[0]] = w->operand.imm[1];
                }
                break;
            case op_movltrr:
                if (regs[cc] < 0) {
                    regs[w->reg[0]] = regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region LDR
            case op_ldrri:
                __builtin_memcpy(&regs[w->reg[0]], map_addr(&obj, w->operand.imm[1]), sizeof(uint32_t));
                break;
            case op_ldrrr:
                __builtin_memcpy(&regs[w->reg[0]], map_addr(&obj, regs[w->reg[1]]), sizeof(uint32_t));
                break;
            case op_ldrrir:
                rel = regs[w->reg[1]] + w->operand.imm[1];
                __builtin_memcpy(&regs[w->reg[0]], map_addr(&obj, rel), sizeof(uint32_t));
                break;
#pragma endregion
#pragma region STRIx
            case op_strii:
                __builtin_memcpy(map_addr(&obj, w->operand.imm[1]), &w->operand.imm[0], sizeof(uint32_t));
                break;
            case op_strir:
                __builtin_memcpy(map_addr(&obj, regs[w->reg[1]]), &w->operand.imm[0], sizeof(uint32_t));
                break;
            case op_striir:
                rel = regs[w->reg[1]] + w->operand.imm[1];
                __builtin_memcpy(map_addr(&obj, rel), &w->operand.imm[0], sizeof(uint32_t));
                break;
#pragma endregion
#pragma region STRRx
            case op_strri:
                __builtin_memcpy(map_addr(&obj, w->operand.imm[1]), &regs[w->reg[0]], sizeof(uint32_t));
                break;
            case op_strrr:
                __builtin_memcpy(map_addr(&obj, regs[w->reg[1]]), &regs[w->reg[0]], sizeof(uint32_t));
                break;
            case op_strrir:
                rel = regs[w->reg[1]] + w->operand.imm[1];
                __builtin_memcpy(map_addr(&obj, rel), &regs[w->reg[0]], sizeof(uint32_t));
                break;
#pragma endregion
#pragma region CMP
            case op_cmpii:
                a = w->operand.imm[0];
                b = w->operand.imm[1];
                regs[cc] = (a > b) - (a < b);
                break;
            case op_cmpir:
                a = w->operand.imm[0];
                b = regs[w->reg[1]];
                regs[cc] = (a > b) - (a < b);
                break;
            case op_cmpri:
                a = regs[w->reg[0]];
                b = w->operand.imm[1];
                regs[cc] = (a > b) - (a < b);
                break;
            case op_cmprr:
                a = regs[w->reg[0]];
                b = regs[w->reg[1]];
                regs[cc] = (a > b) - (a < b);
                break;
#pragma endregion
#pragma region LEA
            case op_learil:
                regs[w->reg[0]] = w->operand.addr + w->operand.imm[1];
                break;
            case op_learl:
                regs[w->reg[0]] = w->operand.addr;
                break;
#pragma endregion
#pragma region MSG
            case op_msg:
                for (msg_t *m = w->operand.head; m; m = m->next) {
                    print_str(&r.out, m->string.data ? m->string : i32_str(regs[m->reg]));
                }
                break;
#pragma endregion
#pragma region JMP
            case op_jmp:
                regs[pc] = w->operand.addr - 1;
                break;
#pragma endregion
#pragma region JCC
            case op_jne:
                if (regs[cc]) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
            case op_jeq:
                if (!regs[cc]) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
            case op_jge:
                if (regs[cc] >= 0) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
            case op_jgt:
                if (regs[cc] > 0) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
            case op_jle:
                if (regs[cc] <= 0) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
            case op_jlt:
                if (regs[cc] < 0) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
#pragma endregion
#pragma region CALL
            case op_call:
                regs[lr] = regs[pc];
                regs[pc] = w->operand.addr - 1;
                break;
#pragma endregion
#pragma region RET
            case op_ret:
                if (regs[lr] < 0) {
                    return r;
                }
                regs[pc] = regs[lr];
                break;
#pragma endregion
#pragma region HALT
            case op_halt:
                r.ok = true;
                return r;
#pragma endregion
        }
    }
}
