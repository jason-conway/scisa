#include "scisa.h"

static bool full_write(int32_t fd, uint8_t *s, size_t len)
{
    for (size_t off = 0; off < len;) {
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
        uint8_t *dst = &o->data[o->len];

        ptrdiff_t i = 0;
        ptrdiff_t j = 0;

        while (i < end - c && j < avail) {
            uint8_t e = c[i];
            if (e == '\\' && i + 1 < end - c) {
                switch (c[i + 1]) {
                    case 't':
                        e = '\t';
                        break;
                    case 'n':
                        e = '\n';
                        break;
                    case 'r':
                        e = '\r';
                        break;
                    case '\\':
                        e = '\\';
                        break;
                    case 'e':
                        e = '\e';
                        break;
                    case '0':
                        flush_output(o);
                        i += 2;
                        c += i;
                        goto next;
                    default:
                        abort();
                }
                i++;
            }
            dst[j++] = e;
            i++;
        }

        c += i;
        o->len += j;
        if (o->len == o->cap) {
            flush_output(o);
        }
    next:;
    }
}


static bool is_mnemonic(str_t s, mnemonic_t *m)
{
    static const str_t names[] = {
        MAP(GEN_STR, MNEMONICS_LIST)
    };
    s = str_lower(s);
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
    s = str_lower(s);
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

    if (*c == '(') {
        r.data = str_span(++c, end);
        r.token = str_span(start, c);
        r.type = tok_lparen;
        return r;
    }

    if (*c == ')') {
        r.data = str_span(++c, end);
        r.token = str_span(start, c);
        r.type = tok_rparen;
        return r;
    }

    if (*c == '\'') {
        for (c++; c < end && *c != '\''; c++);
        if (c == end) {
            return r;
        }
        r.data = str_span(&c[1], end);
        r.token = str_span(&start[1], c);
        r.type = tok_string;
        return r;
    }

    if (*c == '-' || *c == '+') {
        if (str_has_prefix(str_span(&c[1], end), S("0x"))) {
            for (c += 3; c < end && is_hex(*c); c++);
            r.data = str_span(c, end);
            r.token = str_span(start, c);
            if (r.token.len < 4) {
                return r;
            }
        }
        else {
            for (c++; c < end && is_digit(*c); c++);
            r.data = str_span(c, end);
            r.token = str_span(start, c);
            if (r.token.len < 2) {
                return r;
            }
        }
        r.type = tok_integer;
        return r;
    }

    if (str_has_prefix(s, S("0x"))) {
        for (c += 2; c < end && is_hex(*c); c++);
        r.data = str_span(c, end);
        r.token = str_span(start, c);
        r.type = tok_integer;
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
        case m_push:
        case m_pop:
            // op reg
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
        case m_ldr:
        case m_str:
            n->op = op_ldrri + 3 * (m - m_ldr);

            // reg
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

            // start of second operand
            t = lex(t.data);
            switch (t.type) {
                // if second operand is a reg, we're all done
                case tok_register:
                    n->op++; // change to reg-reg variant
                    if (!str_reg(&reg, t.token)) {
                        return NULL;
                    }
                    n->reg[1] = reg;
                    break;
                // but if it's an integer, it could be an offset from another reg
                case tok_integer:
                    if (!str_i32(&imm, t.token)) {
                        return NULL;
                    }
                    n->imm[1] = imm;
                    // now lex and check for left paren
                    // if the operand is just the integer, we have to bail
                    // from here, breaking will cause us to fallthrough
                    // and call lex again
                    t = lex(t.data);
                    switch (t.type) {
                        case tok_newline:
                        case tok_eof:
                            *src = t.data;
                            return n;
                        case tok_lparen:
                            break;
                        default:
                            return NULL;
                    }
                    // last token was '('
                    // ensure next token is register and parse
                    t = lex(t.data);
                    switch (t.type) {
                        case tok_register:
                            n->op += 2; // an elusive op_XXXrir
                            if (!str_reg(&reg, t.token)) {
                                return NULL;
                            }
                            n->reg[1] = reg;
                            break;
                        default:
                            return NULL;
                    }
                    // ensure rparen
                    t = lex(t.data);
                    switch (t.type) {
                        case tok_rparen:
                            break;
                        default:
                            return NULL;
                    }
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

static ast_t parse(str_t src, arena_t *heap, arena_t stack)
{
    ast_t r = { .lineno = 1, };

    token_t t = { .data = src };

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
            case tok_lparen:
            case tok_rparen:
                return r;
            case tok_newline:
                r.lineno++;
                break;
            case tok_eof:
                for (insn_t *n = r.head; n; n = n->next) {
                    if (n->label.data) {
                        vaddr *addr = upsert(&table, n->label, NULL);
                        if (!addr) {
                            r.lineno = n->lineno;
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
                    insn->lineno = r.lineno++;
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
            case op_pushr:
            case op_popr:
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
            case op_ldrri:
            case op_strri:
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
            case op_ldrrr:
            case op_strrr:
            case op_cmprr:
                program[i].reg[0] = n->reg[0];
                program[i].reg[1] = n->reg[1];
                break;
            case op_cmpir:
                program[i].operand.imm = n->imm[0];
                program[i].reg[1] = n->reg[1];
                break;
            case op_ldrrir:
            case op_strrir:
                program[i].operand.imm = n->imm[1];
                program[i].reg[0] = n->reg[0];
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

    enum cfg { STACK_TOP = 1 << 21 };
    uint8_t *stack = alloc(&arena, uint8_t, STACK_TOP);

    int32_t regs[r_max] = { 0 };
    regs[fp] = STACK_TOP;
    regs[sp] = STACK_TOP;
    regs[lr] = -1;
    regs[cc] = 0;
    regs[pc] = 0x0;

    for (;; regs[pc]++) {
        int32_t a = 0;
        int32_t b = 0;
        int32_t rel = 0;
        psw_t *w = &program[regs[pc]];
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
                __builtin_memcpy(&stack[regs[sp]], &regs[w->reg[0]], sizeof(uint32_t));
                break;
#pragma endregion
#pragma region POP
            case op_popr:
                __builtin_memcpy(&regs[w->reg[0]], &stack[regs[sp]], sizeof(uint32_t));
                regs[sp] += sizeof(uint32_t);
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
                        // fallthrough
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
                        // fallthrough
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
                regs[w->reg[0]] <<= ((uint32_t)w->operand.imm & 0x1f);
                break;
            case op_lslrr:
                regs[w->reg[0]] <<= ((uint32_t)regs[w->reg[1]] & 0x1f);
                break;
#pragma endregion
#pragma region LSR
            case op_lsrri:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] >> ((uint32_t)w->operand.imm & 0x1f);
                break;
            case op_lsrrr:
                regs[w->reg[0]] = (uint32_t)regs[w->reg[0]] >> ((uint32_t)regs[w->reg[1]] & 0x1f);
                break;
#pragma endregion
#pragma region ASR
            case op_asrri:
                regs[w->reg[0]] >>= ((uint32_t)w->operand.imm & 0x1f);
                break;
            case op_asrrr:
                regs[w->reg[0]] >>= ((uint32_t)regs[w->reg[1]] & 0x1f);
                break;
#pragma endregion
#pragma region LDR
            case op_ldrri:
                __builtin_memcpy(&regs[w->reg[0]], &stack[w->operand.imm], sizeof(uint32_t));
                break;
            case op_ldrrr:
                __builtin_memcpy(&regs[w->reg[0]], &stack[regs[w->reg[1]]], sizeof(uint32_t));
                break;
            case op_ldrrir:
                rel = regs[w->reg[1]] + w->operand.imm;
                __builtin_memcpy(&regs[w->reg[0]], &stack[rel], sizeof(uint32_t));
                break;
#pragma endregion
#pragma region STR
            case op_strri:
                __builtin_memcpy(&stack[w->operand.imm], &regs[w->reg[0]], sizeof(uint32_t));
                break;
            case op_strrr:
                __builtin_memcpy(&stack[regs[w->reg[1]]], &regs[w->reg[0]], sizeof(uint32_t));
                break;
            case op_strrir:
                rel = regs[w->reg[1]] + w->operand.imm;
                __builtin_memcpy(&stack[rel], &regs[w->reg[0]], sizeof(uint32_t));
                break;
#pragma endregion
#pragma region CMP
            case op_cmpii:
                return r;
            case op_cmpir:
                a = w->operand.imm;
                b = regs[w->reg[1]];
                regs[cc] = (a > b) - (a < b);
                break;
            case op_cmpri:
                a = regs[w->reg[0]];
                b = w->operand.imm;
                regs[cc] = (a > b) - (a < b);
                break;
            case op_cmprr:
                a = regs[w->reg[0]];
                b = regs[w->reg[1]];
                regs[cc] = (a > b) - (a < b);
                break;
#pragma endregion
#pragma region JMP
            case op_jmp:
                regs[pc] = w->operand.addr - 1;
                break;
            case op_jne:
                if (regs[cc]) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
            case op_je:
                if (!regs[cc]) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
            case op_jge:
                if (regs[cc] >= 0) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
            case op_jg:
                if (regs[cc] > 0) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
            case op_jle:
                if (regs[cc] <= 0) {
                    regs[pc] = w->operand.addr - 1;
                }
                break;
            case op_jl:
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
#pragma region MSG
            case op_msg:
                for (msg_t *m = w->operand.head; m; m = m->next) {
                    print_str(&r.out, m->string.data ? m->string : i32_str(regs[m->reg]));
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
    bool err = false;

    err |= fseek(stdin, 0, SEEK_END);
    size_t len = ftell(stdin);
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
        print_str(&err, i32_str(ast.lineno));
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
