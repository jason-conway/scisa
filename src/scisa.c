#include "scisa.h"

static str_t os_loadstdin(arena_t *a);

static bool is_register(str_t s)
{
    const int64_t len = s.len;
    if (!len || len > 3) {
        return false;
    }
    s = str_lower(s);
    if (str_equal(s, S("pc")) || str_equal(s, S("lr"))) {
        return true;
    }
    if (str_equal(s, S("sp")) || str_equal(s, S("fp"))) {
        return true;
    }
    if (str_equal(s, S("cc"))) {
        return true;
    }

    uint8_t *c = &s.data[0];
    if (c[0] != 'r') {
        return false;
    }
    for (int64_t i = 1; i < s.len; i++) {
        if (!is_digit(c[i])) {
            return false;
        }
    }
    return true;
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

static bool is_directive(str_t s, directive_t *d)
{
    static const str_t names[] = {
        MAP(GEN_STR, DIRECTIVE_LIST)
    };
    s = str_lower(s);
    for (size_t i = 0; i < countof(names); i++) {
        if (str_equal(names[i], s)) {
            *d = i + 1;
            return true;
        }
    }
    return false;
}

static bool is_segment_dir(directive_t d, seg_t *seg)
{
    switch (d) {
        case dir_data:
            *seg = seg_data;
            return true;
        case dir_text:
            *seg = seg_text;
            return true;
        default:
            return false;
    }
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

void *__alloc(arena_t *a, size_t objsize, size_t align, size_t count)
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

static sym_addr_t *sym_upsert(labels_t **t, str_t label, arena_t *a)
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
        return r;
    }

    if (is_digit(*c)) {
        for (c++; c < end && is_digit(*c); c++);
        r.data = str_span(c, end);
        r.token = str_span(start, c);
        r.type = tok_integer;
        return r;
    }

    if (*c == '.') {
        for (c++; c < end && is_identifier(*c); c++);
        r.data = str_span(c, end);
        r.token = str_span(&start[1], c);
        r.type = tok_directive;
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


static data_t *parse_directive(arena_t *a, directive_t dir, str_t *src)
{
    token_t t = { .data = *src };
    int32_t val = 0;
    if (dir == dir_text && dir != dir_data) {
        return NULL;
    }

    t = lex(t.data);
    switch (t.type) {
        case tok_integer:
            if (!str_i32(&val, t.token)) {
                return NULL;
            }
            break;
        default:
            return NULL;
    }

    data_t *n = alloc(a, data_t, 1);
    switch (dir) {
        case dir_byte:
            n->val = (uint8_t)val;
            n->sz = 1;
            break;
        case dir_word:
            n->val = (uint32_t)val;
            n->sz = 4;
            break;
        case dir_zero:
            n->val = 0;
            n->sz = val;
            break;
        case dir_align:
            n->align = (uint8_t)val;
            break;
        default:
            return NULL;
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
        case m_movne:
        case m_moveq:
        case m_movge:
        case m_movgt:
        case m_movle:
        case m_movlt:
        case m_movhs:
        case m_movhi:
        case m_movls:
        case m_movlo:
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
#pragma region m_ldr
        case m_ldr:
            n->op = op_ldrri; // [0] reg imm
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
                    n->op++; // [1] reg reg
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
                            n->op += 2;
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
#pragma endregion
#pragma region m_str
        case m_str:
            t = lex(t.data);
            switch (t.type) {
                case tok_register:
                    n->op = op_strri; // reg imm
                    if (!str_reg(&reg, t.token)) {
                        return NULL;
                    }
                    n->reg[0] = reg;
                    break;
                case tok_integer:
                    n->op = op_strii; // imm imm
                    if (!str_i32(&imm, t.token)) {
                        return NULL;
                    }
                    n->imm[0] = imm;
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

            t = lex(t.data);
            switch (t.type) {
                case tok_register:
                    n->op++; // op_strXr
                    if (!str_reg(&reg, t.token)) {
                        return NULL;
                    }
                    n->reg[1] = reg;
                    break;
                case tok_integer:
                    if (!str_i32(&imm, t.token)) {
                        return NULL;
                    }
                    n->imm[1] = imm;
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
                    t = lex(t.data);
                    switch (t.type) {
                        case tok_register:
                            n->op += 2; // op_strXir
                            if (!str_reg(&reg, t.token)) {
                                return NULL;
                            }
                            n->reg[1] = reg;
                            break;
                        default:
                            return NULL;
                    }
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
#pragma endregion
#pragma region m_lea
        case m_lea:
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
            // omnomnom
            t = lex(t.data);
            switch (t.type) {
                case tok_comma:
                    break;
                default:
                    return NULL;
            }

            // now either label or an immediate
            t = lex(t.data);
            switch (t.type) {
                case tok_identifier:
                    n->op = op_learl;
                    // if just label, we done
                    n->label = t.token;
                    break;

                case tok_integer:
                    n->op = op_learil;
                    // grab value
                    if (!str_i32(&imm, t.token)) {
                        return NULL;
                    }
                    n->imm[1] = imm;
                    // invalid syntax if next token isn't lparen
                    t = lex(t.data);
                    switch (t.type) {
                        case tok_lparen:
                            break;
                        default:
                            return NULL;
                    }

                    // error if not a token
                    t = lex(t.data);
                    switch (t.type) {
                        case tok_identifier:
                            n->label = t.token;
                            break;
                        default:
                            return NULL;
                    }
                    // error if not rparen
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
#pragma endregion
#pragma region m_cmp
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
            break;
#pragma endregion
        case m_jmp:
        case m_jne:
        case m_jeq:
        case m_jge:
        case m_jgt:
        case m_jle:
        case m_jlt:
        case m_jhs:
        case m_jhi:
        case m_jls:
        case m_jlo:
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

static vaddr label_addr(sym_addr_t addr)
{
    if (addr > UINT32_MAX) {
        return (vaddr)(addr >> 0x20);
    }
    return (vaddr)addr;
}

static sym_addr_t symbol_addr(seg_t segment, seg_addrs_t addr)
{
    if (unlikely(segment > seg_max)) {
        __builtin_trap();
    }

    const sym_addr_t addrs[] = {
        (uint64_t)-1,
        (uint64_t)(addr.insn + 0x00000000) << 0x00,
        (uint64_t)(addr.data + 0x10000000) << 0x20
    };
    return addrs[segment];
}

static ast_t parse(str_t src, arena_t *heap, arena_t stack)
{
    ast_t r = { .lineno = 1, };

    token_t t = { .data = src };

    seg_addrs_t addr = { 0 };

    labels_t *table = NULL;

    struct seg_tails_t {
        insn_t **insn;
        data_t **data;
    } tail = { &r.head.insn, .data = &r.head.data };

    seg_t seg = seg_null;
    size_t align = DEFAULT_ALIGNMENT;

    for (;;) {
        mnemonic_t m = m_null;
        directive_t dir = dir_null;
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
                for (insn_t *n = r.head.insn; n; n = n->next) {
                    if (n->label.data) {
                        sym_addr_t *sym_addr = sym_upsert(&table, n->label, NULL);
                        if (!sym_addr) {
                            r.lineno = n->lineno;
                            return r;
                        }
                        n->addr = label_addr(*sym_addr);
                    }
                }
                r.ok = true;
                return r;
            case tok_identifier:
                if (is_mnemonic(t.token, &m)) {
                    if (seg == seg_null) {
                        return r;
                    }
                    insn_t *insn = parse_instruction(heap, m, &t.data);
                    if (!insn) {
                        return r;
                    }
                    insn->lineno = r.lineno++;
                    *tail.insn = insn;
                    tail.insn = &(*tail.insn)->next;
                    addr.insn++;
                }
                else { // is label
                    str_t label = t.token;
                    t = lex(t.data);
                    switch (t.type) {
                        case tok_colon:
                            break;
                        default:
                            return r;
                    }
                    sym_addr_t sym = symbol_addr(seg, addr);
                    *sym_upsert(&table, label, &stack) = sym;
                }
                break;
            case tok_directive:
                if (is_directive(t.token, &dir)) {
                    // updates the current section if true
                    if (is_segment_dir(dir, &seg)) {
                        break;
                    }
                    if (seg == seg_text) {
                        return r;
                    }

                    data_t *data = parse_directive(heap, dir, &t.data);
                    if (!data) {
                        return r;
                    }
                    data->lineno = r.lineno++;
                    *tail.data = data;
                    tail.data = &(*tail.data)->next;
                    if (data->align) {
                        align = data->align;
                        break;
                    }
                    addr.data += (data->sz < align) ? align : data->sz;
                    break;
                }
                return r;
        }
    }
}

static uint32_t ds_size(ast_t *ast)
{
    uint8_t align = DEFAULT_ALIGNMENT;
    uint32_t sz = 0;
    for (data_t *d = ast->head.data; d; d = d->next) {
        if (d->align) {
            align = d->align;
            // data entry with non-zero align field has no size
            continue;
        }
        sz += (d->sz < align) ? align : d->sz;
    }
    return sz;
}

static memory_region_t assemble_data(ast_t *ast, arena_t *arena)
{
    memory_region_t r = {
        .size = ds_size(ast)
    };

    fprintf(stderr, "[assemble] allocating %u bytes for data segment\n", r.size);
    r.base = alloc(arena, uint8_t, r.size);

    uint8_t align = DEFAULT_ALIGNMENT;
    size_t offset = 0;

    for (data_t *d = ast->head.data; d; d = d->next) {
        if (d->align) {
            align = d->align;
            continue;
        }
        fprintf(stderr, "[assemble] [%zu] [0x%08x] [%u]\n", offset, d->val, d->sz);
        __builtin_memcpy(&r.base[offset], &d->val, d->sz);
        offset += (d->sz < align) ? align : d->sz;
    }
    return r;
}

static scir_t *assemble_code(ast_t *ast, arena_t *arena)
{
    size_t sz = 0;
    for (insn_t *n = ast->head.insn; n; n = n->next) {
        sz++;
    }

    scir_t *code = alloc(arena, scir_t, sz + 1);

    size_t i = 0;
    for (insn_t *n = ast->head.insn; n; n = n->next) {
        code[i].op = (uint8_t)n->op;
        switch (n->op) {
            case op_abort:
                return NULL;
            case op_ret:
            case op_halt:
                break;
            case op_incr:
            case op_decr:
            case op_negr:
            case op_pushr:
            case op_popr:
                code[i].reg[0] = n->reg[0];
                break;
            case op_movri:
            case op_addri:
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
            case op_movneri:
            case op_moveqri:
            case op_movgeri:
            case op_movgtri:
            case op_movleri:
            case op_movltri:
            case op_movhsri:
            case op_movhiri:
            case op_movlsri:
            case op_movlori:
            case op_ldrri:
            case op_strri:
            case op_cmpri:
                code[i].reg[0] = n->reg[0];
                code[i].operand.imm[1] = n->imm[1];
                break;
            case op_movrr:
            case op_addrr:
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
            case op_movnerr:
            case op_moveqrr:
            case op_movgerr:
            case op_movgtrr:
            case op_movlerr:
            case op_movltrr:
            case op_movhsrr:
            case op_movhirr:
            case op_movlsrr:
            case op_movlorr:
            case op_ldrrr:
            case op_strrr:
            case op_cmprr:
                code[i].reg[0] = n->reg[0];
                code[i].reg[1] = n->reg[1];
                break;
            case op_cmpir:
                code[i].operand.imm[0] = n->imm[0];
                code[i].reg[1] = n->reg[1];
                break;
            case op_cmpii:
                code[i].operand.imm[0] = n->imm[0];
                code[i].operand.imm[1] = n->imm[1];
                break;
            case op_ldrrir:
            case op_strrir:
                code[i].reg[0] = n->reg[0];
                code[i].operand.imm[1] = n->imm[1];
                code[i].reg[1] = n->reg[1];
                break;
            case op_learil:
                code[i].reg[0] = n->reg[0];
                code[i].operand.imm[1] = n->imm[1];
                code[i].operand.addr = n->addr;
                break;
            case op_learl:
                code[i].reg[0] = n->reg[0];
                code[i].operand.addr = n->addr;
                break;
            case op_strii:
                code[i].operand.imm[0] = n->imm[0];
                code[i].operand.imm[1] = n->imm[1];
                break;
            case op_strir:
                code[i].operand.imm[0] = n->imm[0];
                code[i].reg[1] = n->reg[1];
                break;
            case op_striir:
                code[i].operand.imm[0] = n->imm[0];
                code[i].operand.imm[1] = n->imm[1];
                code[i].reg[1] = n->reg[1];
                break;
            case op_jmp:
            case op_jne:
            case op_jeq:
            case op_jge:
            case op_jgt:
            case op_jle:
            case op_jlt:
            case op_jhs:
            case op_jhi:
            case op_jls:
            case op_jlo:
            case op_call:
                code[i].operand.addr = n->addr;
                break;
            case op_msg:
                code[i].operand.head = n->head;
                break;
        }
        i++;
    }
    return code;
}

static scoff_t assemble(ast_t ast, arena_t *arena)
{
    scoff_t obj = { 0 };
    obj.data = assemble_data(&ast, arena);
    obj.code = assemble_code(&ast, arena);
    obj.ok = !!obj.code;
    return obj;
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

    scoff_t program = assemble(ast, &heap);
    if (!program.ok) {
        print_str(&err, S("[error] assembly aborted\n"));
        flush_output(&err);
        return false;
    }

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

// platform specific

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
