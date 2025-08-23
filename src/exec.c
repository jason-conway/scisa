#include "scisa.h"

enum cfg { 
    STACK_HIGH = 0x7fffffff,
    STACK_SIZE = 1u << 21,
    STACK_LOW = STACK_HIGH - STACK_SIZE,
    DATA_LOW = 0x10000000,
};

static size_t ldst_sz(opcode_t op)
{
    switch (op) {
        default:
            __builtin_trap();
        case op_ldsbri:
        case op_ldsbrr:
        case op_ldsbrir:
        case op_ldubri:
        case op_ldubrr:
        case op_ldubrir:
        case op_stbii:
        case op_stbir:
        case op_stbiir:
        case op_stbri:
        case op_stbrr:
        case op_stbrir:
            return 1;
        case op_ldshri:
        case op_ldshrr:
        case op_ldshrir:
        case op_lduhri:
        case op_lduhrr:
        case op_lduhrir:
        case op_sthii:
        case op_sthir:
        case op_sthiir:
        case op_sthri:
        case op_sthrr:
        case op_sthrir:
            return 2;
        case op_ldwri:
        case op_ldwrr:
        case op_ldwrir:
        case op_stwii:
        case op_stwir:
        case op_stwiir:
        case op_stwri:
        case op_stwrr:
        case op_stwrir:
            return 4;
    }
}

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

static uint32_t setcc(uint32_t u0, uint32_t u1)
{
    int32_t i0 = u0;
    int32_t i1 = u1;
    uint32_t r = CC_NULL;
    r |= (u0 != u1) ? CC_NE : CC_NULL;
    r |= (u0 == u1) ? CC_EQ : CC_NULL;
    r |= (i0 >= i1) ? CC_GE : CC_NULL;
    r |= (i0  > i1) ? CC_GT : CC_NULL;
    r |= (i0 <= i1) ? CC_LE : CC_NULL;
    r |= (i0  < i1) ? CC_LT : CC_NULL;
    r |= (u0 >= u1) ? CC_HS : CC_NULL;
    r |= (u0  > u1) ? CC_HI : CC_NULL;
    r |= (u0 <= u1) ? CC_LS : CC_NULL;
    r |= (u0  < u1) ? CC_LO : CC_NULL;
    return r;
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
    regs[cc] = CC_NULL;
    regs[pc] = 0x0;

    // vm cycle counter
    regs[cyc] = 0x0;

    scir_t *insns = obj.code;
    for (;; regs[pc]++) {
        regs[cyc]++;
        int32_t rel = 0; // for imm+reg addressing
        union ext_t {
            int8_t   i8;
            uint8_t  u8;
            int16_t  i16;
            uint16_t u16;
        } ext; // for implicit [s|z]ext
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
                        break;
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
                        break;
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
                        regs[w->reg[0]] = 0;
                        break;
                    case -1:
                        if (regs[w->reg[0]] == INT32_MIN) {
                            regs[w->reg[0]] = INT32_MIN;
                            break;
                        }
                        // fallthrough
                    default:
                        regs[w->reg[0]] /= w->operand.imm[1];
                }
                break;
            case op_sdivrr:
                switch (regs[w->reg[1]]) {
                    case 0:
                        regs[w->reg[0]] = 0;
                        break;
                    case -1:
                        if (regs[w->reg[0]] == INT32_MIN) {
                            regs[w->reg[0]] = INT32_MIN;
                            break;
                        }
                        // fallthrough
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
                if (regs[cc] & CC_NE) {
                    regs[w->reg[0]] = w->operand.imm[1];
                }
                break;
            case op_movnerr:
                if (regs[cc] & CC_NE) {
                    regs[w->reg[0]] = regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region MOVEQ
            case op_moveqri:
                if (regs[cc] & CC_EQ) {
                    regs[w->reg[0]] = w->operand.imm[1];
                }
                break;
            case op_moveqrr:
                if (regs[cc] & CC_EQ) {
                    regs[w->reg[0]] = regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region MOVGE
            case op_movgeri:
                if (regs[cc] & CC_GE) {
                    regs[w->reg[0]] = w->operand.imm[1];
                }
                break;
            case op_movgerr:
                if (regs[cc] & CC_GE) {
                    regs[w->reg[0]] = regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region MOVGT
            case op_movgtri:
                if (regs[cc] & CC_GT) {
                    regs[w->reg[0]] = w->operand.imm[1];
                }
                break;
            case op_movgtrr:
                if (regs[cc] & CC_GT) {
                    regs[w->reg[0]] = regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region MOVLE
            case op_movleri:
                if (regs[cc] & CC_LE) {
                    regs[w->reg[0]] = w->operand.imm[1];
                }
                break;
            case op_movlerr:
                if (regs[cc] & CC_LE) {
                    regs[w->reg[0]] = regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region MOVLT
            case op_movltri:
                if (regs[cc] & CC_LT) {
                    regs[w->reg[0]] = w->operand.imm[1];
                }
                break;
            case op_movltrr:
                if (regs[cc] & CC_LT) {
                    regs[w->reg[0]] = regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region MOVHS
            case op_movhsri:
                if (regs[cc] & CC_HS) {
                    regs[w->reg[0]] = w->operand.imm[1];
                }
                break;
            case op_movhsrr:
                if (regs[cc] & CC_HS) {
                    regs[w->reg[0]] = regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region MOVHI
            case op_movhiri:
                if (regs[cc] & CC_HI) {
                    regs[w->reg[0]] = w->operand.imm[1];
                }
                break;
            case op_movhirr:
                if (regs[cc] & CC_HI) {
                    regs[w->reg[0]] = regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region MOVLS
            case op_movlsri:
                if (regs[cc] & CC_LS) {
                    regs[w->reg[0]] = w->operand.imm[1];
                }
                break;
            case op_movlsrr:
                if (regs[cc] & CC_LS) {
                    regs[w->reg[0]] = regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region MOVLO
            case op_movlori:
                if (regs[cc] & CC_LO) {
                    regs[w->reg[0]] = w->operand.imm[1];
                }
                break;
            case op_movlorr:
                if (regs[cc] & CC_LO) {
                    regs[w->reg[0]] = regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region OUT
            case op_outri:
                xhexdump(map_addr(&obj, regs[w->reg[0]]), w->operand.imm[1]);
                print_str(&r.out, str_from(map_addr(&obj, regs[w->reg[0]]), w->operand.imm[1]));
                break;
            case op_outrr:
                xhexdump(map_addr(&obj, regs[w->reg[0]]), regs[w->reg[1]]);
                print_str(&r.out, str_from(map_addr(&obj, regs[w->reg[0]]), regs[w->reg[1]]));
                break;
#pragma endregion
#pragma region LDSB
            case op_ldsbri:
                __builtin_memcpy(&ext.i8, map_addr(&obj, w->operand.imm[1]), sizeof(int8_t));
                regs[w->reg[0]] = ext.i8;
                break;
            case op_ldsbrr:
                __builtin_memcpy(&ext.i8, map_addr(&obj, regs[w->reg[1]]), sizeof(int8_t));
                regs[w->reg[0]] = ext.i8;
                break;
            case op_ldsbrir:
                rel = regs[w->reg[1]] + w->operand.imm[1];
                __builtin_memcpy(&ext.i8, map_addr(&obj, rel), sizeof(int8_t));
                regs[w->reg[0]] = ext.i8;
                break;
#pragma endregion
#pragma region LDSH
            case op_ldshri:
                __builtin_memcpy(&ext.i16, map_addr(&obj, w->operand.imm[1]), sizeof(int16_t));
                regs[w->reg[0]] = ext.i16;
                break;
            case op_ldshrr:
                __builtin_memcpy(&ext.i16, map_addr(&obj, regs[w->reg[1]]), sizeof(int16_t));
                regs[w->reg[0]] = ext.i16;
                break;
            case op_ldshrir:
                rel = regs[w->reg[1]] + w->operand.imm[1];
                __builtin_memcpy(&ext.i16, map_addr(&obj, rel), sizeof(int16_t));
                regs[w->reg[0]] = ext.i16;
                break;
#pragma endregion
#pragma region LDUB
            case op_ldubri:
                __builtin_memcpy(&ext.u8, map_addr(&obj, w->operand.imm[1]), sizeof(uint8_t));
                regs[w->reg[0]] = ext.u8;
                break;
            case op_ldubrr:
                __builtin_memcpy(&ext.u8, map_addr(&obj, regs[w->reg[1]]), sizeof(uint8_t));
                regs[w->reg[0]] = ext.u8;
                break;
            case op_ldubrir:
                rel = regs[w->reg[1]] + w->operand.imm[1];
                __builtin_memcpy(&ext.u8, map_addr(&obj, rel), sizeof(uint8_t));
                regs[w->reg[0]] = ext.u8;
                break;
#pragma endregion
#pragma region LDUH
            case op_lduhri:
                __builtin_memcpy(&ext.u16, map_addr(&obj, w->operand.imm[1]), sizeof(uint16_t));
                regs[w->reg[0]] = ext.u16;
                break;
            case op_lduhrr:
                __builtin_memcpy(&ext.u16, map_addr(&obj, regs[w->reg[1]]), sizeof(uint16_t));
                regs[w->reg[0]] = ext.u16;
                break;
            case op_lduhrir:
                rel = regs[w->reg[1]] + w->operand.imm[1];
                __builtin_memcpy(&ext.u16, map_addr(&obj, rel), sizeof(uint16_t));
                regs[w->reg[0]] = ext.u16;
                break;
#pragma endregion
#pragma region LDW
            case op_ldwri:
                __builtin_memcpy(&regs[w->reg[0]], map_addr(&obj, w->operand.imm[1]), sizeof(uint32_t));
                break;
            case op_ldwrr:
                __builtin_memcpy(&regs[w->reg[0]], map_addr(&obj, regs[w->reg[1]]), sizeof(uint32_t));
                break;
            case op_ldwrir:
                rel = regs[w->reg[1]] + w->operand.imm[1];
                __builtin_memcpy(&regs[w->reg[0]], map_addr(&obj, rel), sizeof(uint32_t));
                break;
#pragma endregion
#pragma region STWIx
            case op_stbii:
            case op_sthii:
            case op_stwii:
                __builtin_memcpy(map_addr(&obj, w->operand.imm[1]), &w->operand.imm[0], ldst_sz(w->op));
                break;
            case op_stbir:
            case op_sthir:
            case op_stwir:
                __builtin_memcpy(map_addr(&obj, regs[w->reg[1]]), &w->operand.imm[0], ldst_sz(w->op));
                break;
            case op_stbiir:
            case op_sthiir:
            case op_stwiir:
                rel = regs[w->reg[1]] + w->operand.imm[1];
                __builtin_memcpy(map_addr(&obj, rel), &w->operand.imm[0], ldst_sz(w->op));
                break;
#pragma endregion
#pragma region STWRx
            case op_stbri:
            case op_sthri:
            case op_stwri:
                __builtin_memcpy(map_addr(&obj, w->operand.imm[1]), &regs[w->reg[0]], ldst_sz(w->op));
                break;
            case op_stbrr:
            case op_sthrr:
            case op_stwrr:
                __builtin_memcpy(map_addr(&obj, regs[w->reg[1]]), &regs[w->reg[0]], ldst_sz(w->op));
                break;
            case op_stbrir:
            case op_sthrir:
            case op_stwrir:
                rel = regs[w->reg[1]] + w->operand.imm[1];
                __builtin_memcpy(map_addr(&obj, rel), &regs[w->reg[0]], ldst_sz(w->op));
                break;
#pragma endregion
#pragma region CMP
            case op_cmpii:
                regs[cc] = setcc(w->operand.imm[0], w->operand.imm[1]);
                break;
            case op_cmpir:
                regs[cc] = setcc(w->operand.imm[0], regs[w->reg[1]]);
                break;
            case op_cmpri:
                regs[cc] = setcc(regs[w->reg[0]], w->operand.imm[1]);
                break;
            case op_cmprr:
                regs[cc] = setcc(regs[w->reg[0]], regs[w->reg[1]]);
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
#pragma region JNE
            case op_jne:
                if (regs[cc] & CC_NE) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
#pragma endregion
#pragma region JEQ
            case op_jeq:
                if (regs[cc] & CC_EQ) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
#pragma endregion
#pragma region JGE
            case op_jge:
                if (regs[cc] & CC_GE) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
#pragma endregion
#pragma region JGT
            case op_jgt:
                if (regs[cc] & CC_GT) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
#pragma endregion
#pragma region JLE
            case op_jle:
                if (regs[cc] & CC_LE) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
#pragma endregion
#pragma region JLT
            case op_jlt:
                if (regs[cc] & CC_LT) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
#pragma endregion
#pragma region JHS
            case op_jhs:
                if (regs[cc] & CC_HS) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
#pragma endregion
#pragma region JHI
            case op_jhi:
                if (regs[cc] & CC_HI) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
#pragma endregion
#pragma region JLS
            case op_jls:
                if (regs[cc] & CC_LS) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
#pragma endregion
#pragma region JLO
            case op_jlo:
                if (regs[cc] & CC_LO) {
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
