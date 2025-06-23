#include "scisa.h"

static bool full_write(int32_t fd, uint8_t *s, ptrdiff_t len)
{
    for (ptrdiff_t off = 0; off < len;) {
        ptrdiff_t r = write(fd, &s[off], len - off);
        if (r < 1) {
            return false;
        }
        off += r;
    }
    return true;
}

static void flush_output(output_t *o)
{
    if (!o->err && o->len) {
        o->err = !full_write(o->fd, o->data, o->len);
        o->len = 0;
    }
}

static void print_str(output_t *o, str_t s)
{
    uint8_t *c = &s.data[0];
    uint8_t *end = &s.data[s.len];
    while (c < end && !o->err) {
        ptrdiff_t avail = o->cap - o->len;
        ptrdiff_t count = avail < end - c ? avail : end - c;
        uint8_t *dst = &o->data[o->len];
        ptrdiff_t j = 0;
        ptrdiff_t i = 0;
        for (; i < s.len && j < count; i++) {
            uint8_t e = c[i];
            if (e == '\\' && i < count - 1) {
                switch (c[++i]) {
                    case 't':
                        e = '\t';
                        break;
                    case 'n':
                        e = '\n';
                        break;
                    case '\\':
                        e = '\\';
                        break;
                    case 'e':
                        e = '\e';
                        break;
                    default:
                        abort();
                }
            }
            dst[j++] = e;
        }
        c += i;
        o->len += j;
        if (count == s.len) {
            flush_output(o);
            continue;
        }
        break;
    }
}

static void print_i32(output_t *o, int32_t v)
{
    uint8_t data[16] = { 0 };
    uint8_t *end = &data[countof(data)];
    uint8_t *start = end;
    int32_t i = v < 0 ? v : -v;
    do {
        *--start = '0' - (uint8_t)(i % 10);
    } while (i /= 10);
    if (v < 0) {
        *--start = '-';
    }
    print_str(o, str_span(start, end));
}

static bool is_mnemonic(str_t s, mnemonic_t *m)
{
    static const str_t names[] = {
        MAP(GEN_STR, MNEMONICS_LIST)
    };
    for (size_t i = 0; i < countof(names); i++) {
        if (str_equal(names[i], s)) {
            *m = i + 1;
            return true;
        }
    }
    return false;
}

static bool str_reg(reg_t *r, str_t s)
{
    static const str_t names[] = {
        MAP(GEN_STR, REGISTER_LIST)
    };
    for (size_t i = 0; i < countof(names); i++) {
        if (str_equal(names[i], s)) {
            *r = i;
            return true;
        }
    }
    return false;
}

static void *__alloc(arena_t *a, size_t objsize, size_t align, size_t count)
{
    ptrdiff_t avail = a->end - a->start;
    ptrdiff_t padding = -(uintptr_t)a->start & (align - 1);
    if (count > (avail - padding) / objsize) {
        abort();
    }
    size_t total = objsize * count;

    uint8_t *r = &a->start[padding];
    for (size_t i = 0; i < total; i++) {
        a->start[padding + i] = 0;
    }
    a->start += padding + total;
    return r;
}

static vaddr *upsert(labels_t **t, str_t label, arena_t *a)
{
    for (uint64_t h = str_hash(label); *t; h = h >> 62 | h << 2) {
        if (str_equal((*t)->label, label)) {
            return &(*t)->addr;
        }
        // set `t` to next child node
        t = &(*t)->child[h >> 62];
    }
    if (!a) {
        return NULL;
    }
    *t = alloc(a, labels_t, 1);
    (*t)->label = label;
    return &(*t)->addr;
}


static token_t lex(str_t s)
{
    token_t r = { 0 };

    for (;;) {
        s = str_trim(s);
        if (!s.len) {
            r.type = tok_eof;
            return r;
        }
        else if (*s.data == ';') {
            uint8_t *data = s.data;
            uint8_t *end = &s.data[s.len];
            for (data++; data < end && *data != '\n'; data++);
            s = str_span(data, end);
            continue;
        }
        break;
    }

    uint8_t *start = s.data;
    uint8_t *end = &s.data[s.len];
    uint8_t *c = start;

    if (*c == '\n') {
        r.data = str_span(++c, end);
        r.token = str_span(start, c);
        r.type = tok_newline;
        return r;
    }

    if (*c == ',') {
        r.data = str_span(++c, end);
        r.token = str_span(start, c);
        r.type = tok_comma;
        return r;
    }

    if (*c == ':') {
        r.data = str_span(++c, end);
        r.token = str_span(start, c);
        r.type = tok_colon;
        return r;
    }

    if (*c == '\'') {
        for (c++; c < end &&  *c != '\''; c++);
        if (c == end) {
            return r;
        }
        r.data = str_span(&c[1], end);
        r.token = str_span(&start[1], c);
        r.type = tok_string;
        return r;
    }

    if (*c == '-' || *c == '+') {
        for (c++; c < end && is_digit(*c); c++);
        r.data = str_span(c, end);
        r.token = str_span(start, c);
        if (r.token.len < 2) {
            return r;
        }
        r.type = tok_integer;
        return r;
    }

    if (is_digit(*c)) {
        for (c++; c < end && is_digit(*c); c++);
        r.data = str_span(c, end);
        r.token = str_span(start, c);
        r.type = tok_integer;
        return r;
    }

    if (*c == '_' || is_letter(*c)) {
        for (c++; c < end && is_identifier(*c); c++);
        r.data = str_span(c, end);
        r.token = str_span(start, c);
        r.type = is_register(r.token) ? tok_register : tok_identifier;
        return r;
    }

    return r;
}

static insn_t *parse_instruction(arena_t *a, mnemonic_t m, str_t *src)
{
    insn_t *n = alloc(a, insn_t, 1);
    msg_t **tail = &n->head;

    token_t t = { .data = *src };

    int32_t imm = -1;
    reg_t reg = r_max;

    switch (m) {
        case m_null:
            return NULL;
        case m_inc:
        case m_dec:
        case m_neg:
            // One opcode per mnemonic
            n->op = op_incr + (m - m_inc);
            t = lex(t.data);
            switch (t.type) {
                case tok_register:
                    if (!str_reg(&reg, t.token)) {
                        return NULL;
                    }
                    n->reg[0] = reg;
                    break;
                default:
                    return NULL;
            }
            break;
        case m_ret:
            n->op = op_ret;
            break;
        case m_halt:
            n->op = op_halt;
            break;
        case m_mov:
        case m_add:
        case m_sadd:
        case m_sub:
        case m_mul:
        case m_div:
        case m_sdiv:
        case m_mod:
        case m_smod:
        case m_and:
        case m_or:
        case m_xor:
        case m_lsl:
        case m_lsr:
        case m_asr:
            // op will start as imm variant of `m`
            n->op = op_movri + 2 * (m - m_mov);

            // First operand must be from reg
            t = lex(t.data);
            switch (t.type) {
                case tok_register:
                    if (!str_reg(&reg, t.token)) {
                        return NULL;
                    }
                    n->reg[0] = reg;
                    break;
                default:
                    return NULL;
            }
            // Eat comma
            t = lex(t.data);
            switch (t.type) {
                case tok_comma:
                    break;
                default:
                    return NULL;
            }
            // Second operand can be immediate or from reg
            t = lex(t.data);
            switch (t.type) {
                case tok_integer:
                    if (!str_i32(&imm, t.token)) {
                        return NULL;
                    }
                    n->imm[1] = imm;
                    break;
                case tok_register:
                    n->op++; // change to reg-reg variant
                    if (!str_reg(&reg, t.token)) {
                        return NULL;
                    }
                    n->reg[1] = reg;
                    break;
                default:
                    return NULL;
            }
            break;
        case m_cmp:
            // First operand can be imm or reg
            t = lex(t.data);
            switch (t.type) {
                case tok_integer:
                    n->op = op_cmpii;
                    if (!str_i32(&imm, t.token)) {
                        return NULL;
                    }
                    n->imm[0] = imm;
                    break;
                case tok_register:
                    n->op = op_cmpri;
                    if (!str_reg(&reg, t.token)) {
                        return NULL;
                    }
                    n->reg[0] = reg;
                    break;
                default:
                    return NULL;
            }
            t = lex(t.data);
            switch (t.type) {
                case tok_comma:
                    break;
                default:
                    return NULL;
            }
            // Second operand can be imm or reg
            t = lex(t.data);
            switch (t.type) {
                case tok_integer:
                    if (!str_i32(&imm, t.token)) {
                        return NULL;
                    }
                    n->imm[1] = imm;
                    break;
                case tok_register:
                    n->op++;
                    if (!str_reg(&reg, t.token)) {
                        return NULL;
                    }
                    n->reg[1] = reg;
                    break;
                default:
                    return NULL;
            }
            if (n->op == op_cmpii) {
                return NULL;
            }
            break;
        case m_jmp:
        case m_jne:
        case m_je:
        case m_jge:
        case m_jg:
        case m_jle:
        case m_jl:
        case m_call:
            // Single label operand
            t = lex(t.data);
            if (t.type != tok_identifier) {
                return NULL;
            }
            n->label = t.token;
            n->op = op_jne + (m - m_jne);
            break;
        case m_msg:
            n->op = op_msg;
            for (tok_t last = tok_error;;) {
                t = lex(t.data);
                switch (t.type) {
                    case tok_newline:
                    case tok_eof:
                        if (last != tok_comma) {
                            *src = t.data;
                            return n;
                        }
                        return NULL;
                    case tok_string:
                        if (last && last != tok_comma) {
                            return NULL;
                        }
                        *tail = alloc(a, msg_t, 1);
                        (*tail)->string = t.token;
                        tail = &(*tail)->next;
                        break;
                    case tok_register:
                        *tail = alloc(a, msg_t, 1);
                        if (!str_reg(&reg, t.token)) {
                            return NULL;
                        }
                        (*tail)->reg = reg;
                        tail = &(*tail)->next;
                        break;
                    case tok_comma:
                        if (!last || last == tok_comma) {
                            return NULL;
                        }
                        break;
                    default:
                        return NULL;
                }
                last = t.type;
            }
        __builtin_trap();
    }

    t = lex(t.data);
    switch (t.type) {
        case tok_eof:
        case tok_newline:
            *src = t.data;
            return n;
        default:
            return NULL;
    }
}

// Note: returns pointers into the source buffer.
static ast_t parse(str_t src, arena_t *heap, arena_t stack)
{
    ast_t r = { 0
        // .lineno = 1,
    };

    token_t t = {
        .data = src
    };

    vaddr addr = 0;
    labels_t *table = NULL;
    insn_t **tail = &r.head;

    for (;;) {
        mnemonic_t m = m_null;
        t = lex(t.data);
        switch (t.type) {
            case tok_error:
            case tok_comma:
            case tok_colon:
            case tok_integer:
            case tok_register:
            case tok_string:
                return r;
            case tok_newline:
                // r.lineno++;
                break;
            case tok_eof:
                for (insn_t *n = r.head; n; n = n->next) {
                    if (n->label.data) {
                        vaddr *addr = upsert(&table, n->label, NULL);
                        if (!addr) {
                            // r.lineno = n->lineno;
                            return r;
                        }
                        n->addr = *addr;
                    }
                }
                r.ok = true;
                return r;

            case tok_identifier:
                if (is_mnemonic(t.token, &m)) {
                    insn_t *insn = parse_instruction(heap, m, &t.data);
                    if (!insn) {
                        return r;
                    }
                    // insn->lineno = r.lineno++;
                    *tail = insn;
                    tail = &(*tail)->next;
                    addr++;
                }
                else { // is label
                    str_t label = t.token;
                    t = lex(t.data);
                    if (t.type != tok_colon) {
                        return r;
                    }
                    *upsert(&table, label, &stack) = addr;
                }
                break;
        }
    }
}

// Note: retains references to the AST.
static psw_t *assemble(insn_t *head, arena_t *arena)
{
    ptrdiff_t len = 0;
    for (insn_t *n = head; n; n = n->next) {
        len++;
    }

    psw_t *program = alloc(arena, psw_t, len + 1);

    size_t i = 0;
    for (insn_t *n = head; n; n = n->next) {
        program[i].op = (uint8_t)n->op;
        switch (n->op) {
            case op_abort:
            case op_cmpii:
                return NULL;
            case op_ret:
            case op_halt:
                break;
            case op_incr:
            case op_decr:
            case op_negr:
                program[i].reg[0] = n->reg[0];
                break;
            case op_movri:
            case op_addri:
            case op_saddri:
            case op_subri:
            case op_mulri:
            case op_divri:
            case op_sdivri:
            case op_modri:
            case op_smodri:
            case op_andri:
            case op_orri:
            case op_xorri:
            case op_lslri:
            case op_lsrri:
            case op_asrri:
            case op_cmpri:
                program[i].reg[0] = n->reg[0];
                program[i].operand.imm = n->imm[1];
                break;
            case op_movrr:
            case op_addrr:
            case op_saddrr:
            case op_subrr:
            case op_mulrr:
            case op_divrr:
            case op_sdivrr:
            case op_modrr:
            case op_smodrr:
            case op_andrr:
            case op_orrr:
            case op_xorrr:
            case op_lslrr:
            case op_lsrrr:
            case op_asrrr:
            case op_cmprr:
                program[i].reg[0] = n->reg[0];
                program[i].reg[1] = n->reg[1];
                break;
            case op_cmpir:
                program[i].operand.imm = n->imm[0];
                program[i].reg[1] = n->reg[1];
                break;
            case op_jmp:
            case op_jne:
            case op_je:
            case op_jge:
            case op_jg:
            case op_jle:
            case op_jl:
            case op_call:
                program[i].operand.addr = n->addr;
                break;
            case op_msg:
                program[i].operand.head = n->head;
                break;
        }
        i++;
    }

    return program;
}

static result_t execute(psw_t *program, arena_t arena)
{
    result_t r = { 0 };
    r.out.fd = STDOUT_FILENO;
    r.out.cap = 1 << 6;
    r.out.data = alloc(&arena, uint8_t, r.out.cap);

    ptrdiff_t len = 0;
    ptrdiff_t cap = 1 << 10;
    ptrdiff_t *stack = alloc(&arena, uint8_t, cap);

    int32_t cmp = 0;
    int32_t regs[r_max] = { 0 };

    for (ptrdiff_t i = 0;; i++) {
        int32_t a = 0;
        int32_t b = 0;
        psw_t *w = &program[i];
        switch (w->op) {
#pragma region ABORT
            case op_abort:
                return r;
#pragma endregion
#pragma region INC/DEC/NEG
            case op_incr:
                regs[w->reg[0]] += (uint32_t)1;
                break;
            case op_decr:
                regs[w->reg[0]] -= (uint32_t)1;
                break;
            case op_negr:
                regs[w->reg[0]] = -regs[w->reg[0]];
                break;
#pragma endregion
#pragma region MOV
            case op_movri:
                regs[w->reg[0]] = w->operand.imm;
                break;
            case op_movrr:
                regs[w->reg[0]] = regs[w->reg[1]];
                break;
#pragma endregion
#pragma region ADD
            case op_addri:
                regs[w->reg[0]] += (uint32_t)w->operand.imm;
                break;
            case op_addrr:
                regs[w->reg[0]] += (uint32_t)regs[w->reg[1]];
                break;
#pragma endregion
#pragma region SADD
            case op_saddri:
                regs[w->reg[0]] += w->operand.imm;
                break;
            case op_saddrr:
                regs[w->reg[0]] += regs[w->reg[1]];
                break;
#pragma endregion
#pragma region SUB
            case op_subri:
                regs[w->reg[0]] -= (uint32_t)w->operand.imm;
                break;
            case op_subrr:
                regs[w->reg[0]] -= (uint32_t)regs[w->reg[1]];
                break;
#pragma endregion
#pragma region MUL
            case op_mulri:
                regs[w->reg[0]] *= (uint32_t)w->operand.imm;
                break;
            case op_mulrr:
                regs[w->reg[0]] *= (uint32_t)regs[w->reg[1]];
                break;
#pragma endregion
#pragma region DIV
            case op_divri:
                switch (w->operand.imm) {
                    case 0:
                        return r;
                    case -1:
                        regs[w->reg[0]] = -(uint32_t)regs[w->reg[0]];
                        break;
                    default:
                        regs[w->reg[0]] /= (uint32_t)w->operand.imm;
                }
                break;
            case op_divrr:
                switch (regs[w->reg[1]]) {
                    case 0:
                        return r;
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
                switch (w->operand.imm) {
                    case 0:
                        return r;
                    case -1:
                        regs[w->reg[0]] = -(uint32_t)regs[w->reg[0]];
                        break;
                    default:
                        regs[w->reg[0]] /= w->operand.imm;
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
                switch (w->operand.imm) {
                    case 0:
                        break;
                    default:
                        regs[w->reg[0]] %= (uint32_t)w->operand.imm;
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
                switch (w->operand.imm) {
                    case 0:
                        break;
                    case -1:
                        if (regs[w->reg[0]] == INT32_MIN) {
                            regs[w->reg[0]] = 0;
                            break;
                        }
                    default:
                        regs[w->reg[0]] %= w->operand.imm;
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
                    default:
                        regs[w->reg[0]] %= regs[w->reg[1]];
                }
                break;
#pragma endregion
#pragma region AND
            case op_andri:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] & (uint32_t)w->operand.imm;
                break;
            case op_andrr:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] & (uint32_t)regs[w->reg[1]];
                break;
#pragma endregion
#pragma region OR
            case op_orri:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] | (uint32_t)w->operand.imm;
                break;
            case op_orrr:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] | (uint32_t)regs[w->reg[1]];
                break;
#pragma endregion
#pragma region XOR
            case op_xorri:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] ^ (uint32_t)w->operand.imm;
                break;

            case op_xorrr:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] ^ (uint32_t)regs[w->reg[1]];
                break;
#pragma endregion
#pragma region LSL
            case op_lslri:
                regs[w->reg[0]] <<= (uint32_t)w->operand.imm;
                break;
            case op_lslrr:
                regs[w->reg[0]] <<= (uint32_t)regs[w->reg[1]];
                break;
#pragma endregion
#pragma region LSR
            case op_lsrri:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] >> (uint32_t)w->operand.imm;
                break;
            case op_lsrrr:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] >> (uint32_t)regs[w->reg[1]];
                break;
#pragma endregion
#pragma region ASR
            case op_asrri:
                regs[w->reg[0]] >>= (uint32_t)w->operand.imm;
                break;
            case op_asrrr:
                regs[w->reg[0]] >>= (uint32_t)regs[w->reg[1]];
                break;
#pragma endregion
#pragma region CMP
            case op_cmpii:
                return r;
            case op_cmpir:
                a = w->operand.imm;
                b = regs[w->reg[1]];
                cmp = (a > b) - (a < b);
                break;
            case op_cmpri:
                a = regs[w->reg[0]];
                b = w->operand.imm;
                cmp = (a > b) - (a < b);
                break;
            case op_cmprr:
                a = regs[w->reg[0]];
                b = regs[w->reg[1]];
                cmp = (a > b) - (a < b);
                break;
#pragma endregion
#pragma region JMP
            case op_jmp:
                i = w->operand.addr - 1;
                break;
            case op_jne:
                if (cmp) {
                    i = w->operand.addr - 1;
                }
                break;
            case op_je:
                if (!cmp) {
                    i = w->operand.addr - 1;
                }
                break;
            case op_jge:
                if (cmp >= 0) {
                    i = w->operand.addr - 1;
                }
                break;
            case op_jg:
                if (cmp > 0) {
                    i = w->operand.addr - 1;
                }
                break;
            case op_jle:
                if (cmp <= 0) {
                    i = w->operand.addr - 1;
                }
                break;
            case op_jl:
                if (cmp < 0) {
                    i = w->operand.addr - 1;
                }
                break;
#pragma endregion
#pragma region CALL
            case op_call:
                if (len == cap) {
                    return r;
                }
                stack[len++] = i;
                i = w->operand.addr - 1;
                break;
#pragma endregion
#pragma region RET
            case op_ret:
                if (!len) {
                    return r;
                }
                i = stack[--len];
                break;
#pragma endregion
#pragma region MSG
            case op_msg:
                for (msg_t *m = w->operand.head; m; m = m->next) {
                    if (m->string.data) {
                        print_str(&r.out, m->string);
                    }
                    else {
                        print_i32(&r.out, regs[m->reg]);
                    }
                }
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


static str_t os_loadstdin(arena_t *a)
{
    str_t s = { 0 };
    bool err = 0;

    err |= fseek(stdin, 0, SEEK_END);
    long len = ftell(stdin);
    err |= len < 1;
    err |= fseek(stdin, 0, SEEK_SET);
    if (err) {
        return s;
    }

    s.data = alloc(a, uint8_t, len);
    s.len = fread(s.data, 1, len, stdin);
    return s;
}

static bool run(arena_t heap)
{
    arena_t a = { 0 };
    ptrdiff_t cap = (heap.end - heap.start) / 2;

    a.start = alloc(&heap, uint8_t, cap);
    a.end = &a.start[cap];

    output_t err = { 0 };
    err.fd = STDERR_FILENO;
    err.cap = 1 << 8;
    err.data = alloc(&heap, uint8_t, err.cap);

    str_t src = os_loadstdin(&heap);

    ast_t ast = parse(src, &heap, a);
    if (!ast.ok) {
        print_str(&err, S("[error] invalid line:"));
        // print_i32(&err, (int32_t)ast.lineno);
        print_str(&err, S("\n"));
        flush_output(&err);
        return false;
    }

    psw_t *program = assemble(ast.head, &heap);
    result_t result = execute(program, a);
    if (!result.ok) {
        print_str(&err, S("[error] execution aborted\n"));
        flush_output(&err);
        return false;
    }
    flush_output(&result.out);
    return true;
}

static arena_t bss_arena(void)
{
    static uint8_t mem[1u << 25];
    arena_t r = { 0 };
    r.start = mem;
    asm("" : "+r"(r.start));
    r.end = &r.start[countof(mem)];
    return r;
}

int main(void)
{
    arena_t heap = bss_arena();
    return !run(heap);
}
