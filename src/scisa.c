#include "scisa.h"

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

static vaddr *upsert_data(labels_t **t, str_t label, arena_t *a)
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

    if (*c == '.') {
        for (c++; c < end && is_identifier(*c); c++);
        r.data = str_span(c, end);
        r.token = str_span(&start[1], c);
        if (!is_asmdir(r.token)) {
            return r;
        }
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


static data_t *parse_data_directive(arena_t *a, directive_t dir, str_t *src)
{
    data_t *n = alloc(a, data_t, 1);
    token_t t = { .data = *src };
    int32_t val = 0;

    switch (dir) {
        case dir_word:
        case dir_byte:
            t = lex(t.data);
            switch (t.type) {
                case tok_integer:
                    if (!str_i32(&val, t.token)) {
                        return NULL;
                    }
                    n->val = val;
                    break;
                default:
                    return NULL;
            }
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
        case m_lda:
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

            t = lex(t.data);
            if (t.type != tok_identifier) {
                return NULL;
            }
            n->label = t.token;
            n->op = op_ldarl;
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

    vaddr insn_addr = 0;
    vaddr data_addr = 0;


    labels_t *table = NULL;
    labels_t *data_table = NULL;

    insn_t **insn_tail = &r.insn_head;
    data_t **data_tail = &r.data_head;

    seg_t section = seg_null;

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
                for (insn_t *n = r.insn_head; n; n = n->next) {
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
                    *insn_tail = insn;
                    insn_tail = &(*insn_tail)->next;
                    insn_addr++;
                }
                else { // is label
                    str_t label = t.token;
                    t = lex(t.data);
                    if (t.type != tok_colon) {
                        return r;
                    }
                    if (section == seg_text) {
                        *upsert(&table, label, &stack) = insn_addr;
                    } else {
                        *upsert(&table, label, &stack) = data_addr;
                    }
                }
                break;
            case tok_directive:
                if (is_directive(t.token, &dir)) {
                    // updates the current section if true
                    if (is_segment_dir(dir, &section)) {
                        break;
                    }
                    if (section == seg_text) {
                        return r;
                    }

                    data_t *data = parse_data_directive(heap, dir, &t.data);
                    if (!data) {
                        return r;
                    }
                    data->lineno = r.lineno++;
                    *data_tail = data;
                    data_tail = &(*data_tail)->next;
                    data_addr++;
                    break;
                }
                return r;
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
            case op_ldarl:
                program[i].operand.addr = n->addr;
                program[i].reg[0] = n->reg[0];
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

    psw_t *program = assemble(ast.insn_head, &heap);
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
