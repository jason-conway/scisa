#include "scisa.h"
#include "std.h"

__malloc
static void *__alloc(arena_t *a, ptrdiff_t objsize, ptrdiff_t align, ptrdiff_t count)
{
    ptrdiff_t avail = a->end - a->start;
    ptrdiff_t padding = -(uintptr_t)a->start & (align - 1);
    if (count > (avail - padding) / objsize) {
        abort();
    }
    ptrdiff_t total = objsize * count;

    uint8_t *r = &a->start[padding];
    for (ptrdiff_t i = 0; i < total; i++) {
        a->start[padding + i] = 0;
    }
    a->start += padding + total;
    return r;
}

typedef enum opcode_t {
    op_abort,

    op_incr,
    op_decr,
    op_negr,

    // [note] ensure order:
    // op_XXXri
    // op_XXXrr

    op_movri,
    op_movrr,

    op_addri,
    op_addrr,

    op_saddri,
    op_saddrr,

    op_subri,
    op_subrr,

    op_mulri,
    op_mulrr,

    op_divri,
    op_divrr,

    op_sdivri,
    op_sdivrr,

    op_modri,
    op_modrr,

    op_smodri,
    op_smodrr,

    op_andri,
    op_andrr,

    op_orri,
    op_orrr,

    op_xorri,
    op_xorrr,

    op_lslri,
    op_lslrr,

    op_lsrri,
    op_lsrrr,

    op_asrri,
    op_asrrr,


    op_cmpii,
    op_cmpir,

    op_cmpri,
    op_cmprr,

    op_jmp,

    op_jne,
    op_je,
    op_jge,
    op_jg,
    op_jle,
    op_jl,

    op_call,
    op_ret,
    op_msg,
    op_end
} opcode_t;

#pragma region MNEMONICS
#define MNEMONICS_LIST \
    inc,    \
    dec,    \
    neg,    \
    ret,    \
    end,    \
    mov,    \
    add,    \
    sadd,   \
    sub,    \
    mul,    \
    div,    \
    sdiv,   \
    mod,    \
    smod,   \
    and,    \
    or,     \
    xor,    \
    lsl,    \
    lsr,    \
    asr,    \
    cmp,    \
    jmp,    \
    jne,    \
    je,     \
    jge,    \
    jg,     \
    jle,    \
    jl,     \
    call,   \
    msg,
#pragma endregion

#define GEN_MNEMONIC_ID(m)  m_##m
#define GEN_MNEMONIC_STR(m) E(STR(m))


typedef enum mnemonic_t {
    m_null,
    MAP(GEN_MNEMONIC_ID, MNEMONICS_LIST)
} mnemonic_t;

#if 0
    m_inc,  // inc R
    m_dec,  // dec R
    m_neg,  // neg R
    m_ret,  // ret
    m_end,  // end  successful program halt
    m_mov,  // mov  R, R|I
    m_add,  // add  R, R|I
    m_sadd, // sadd R, R|I
    m_sub,  // sub  R, R|I
    m_mul,  // mul  R, R|I
    m_div,  // div  R, R|I
    m_sdiv, // sdiv R, R|I
    m_mod,  // mod  R, R|I
    m_smod, // smod R, R|I
    m_and,  // and  R, R|I
    m_or,   // or   R, R|I
    m_xor,  // xor  R, R|I
    m_lsl,  // lsl  R, R|I
    m_lsr,  // lsr  R, R|I
    m_asr,  // asr  R, R|I
    m_cmp,  // cmp  [R, I]|[I, R]|[R, R]
    m_jmp,  // jmp  L
    m_jne,  // jne  L
    m_je,   // je   L
    m_jge,  // jge  L
    m_jg,   // jg   L
    m_jle,  // jle  L
    m_jl,   // jl   L
    m_call, // call L
    m_msg,  // msg [string|R, *]   print operands
#endif
#if 0
        E("inc"),
        E("dec"),
        E("neg"),
        E("ret"),
        E("end"),
        E("mov"),
        E("add"),
        E("sadd"),
        E("sub"),
        E("mul"),
        E("div"),
        E("sdiv"),
        E("mod"),
        E("smod"),
        E("and"),
        E("or"),
        E("xor"),
        E("lsl"),
        E("lsr"),
        E("asr"),
        E("cmp"),
        E("jmp"),
        E("jne"),
        E("je"),
        E("jge"),
        E("jg"),
        E("jle"),
        E("jl"),
        E("call"),
        E("msg"),
#endif

static mnemonic_t parse_mnemonic(str_t s)
{
    static const str_t names[] = {
        MAP(GEN_MNEMONIC_STR, MNEMONICS_LIST)
    };
    for (size_t i = 0; i < countof(names); i++) {
        if (str_equal(names[i], s)) {
            return i + 1;
        }
    }
    return m_null;
}

typedef struct insn_t insn_t;
struct insn_t {
    insn_t *next;
    msg_t *head;
    str_t label;
    ptrdiff_t addr;
    ptrdiff_t lineno;
    opcode_t op;
    int32_t imm[2];
    uint8_t reg[2];
};

typedef struct ast_t {
    insn_t *head;
    ptrdiff_t lineno;
    bool ok;
} ast_t;

typedef struct insnresult_t {
    str_t src;
    insn_t *insn;
} insnresult_t;


typedef struct psw_t {
    uint8_t op;
    uint8_t reg[2];
    union operand_t {
        int32_t imm;
        ptrdiff_t addr;
        msg_t *head;
    } operand;
} psw_t;

typedef struct status_t {
    str_t out;
    str_t err;
    int32_t status;
} status_t;

typedef struct output_t {
    uint8_t *data;
    ptrdiff_t len;
    ptrdiff_t cap;
    bool err;
} output_t;

static void print_str(output_t *o, str_t s)
{
    ptrdiff_t avail = o->cap - o->len;
    ptrdiff_t count = s.len < avail ? s.len : avail;
    uint8_t *dst = &o->data[o->len];
    for (ptrdiff_t i = 0; i < count; i++) {
        dst[i] = s.data[i];
    }
    o->len += count;
    o->err |= count != s.len;
}

static void print_i32(output_t *o, int32_t v)
{
    uint8_t data[16] = { 0 };
    uint8_t *end = &data[countof(data)];
    uint8_t *start = end;
    int32_t t = v < 0 ? v : -v;
    do {
        *--start = '0' - (uint8_t)(t % 10);
    } while (t /= 10);
    if (v < 0) {
        *--start = '-';
    }
    print_str(o, str_span(start, end));
}

typedef struct result_t {
    output_t out;
    bool ok;
} result_t;

typedef struct labels_t labels_t;
struct labels_t {
    labels_t *child[4];
    str_t label;
    uintptr_t addr;
};

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
    uint8_t *data = start;

    if (*data == '\n') {
        r.data = str_span(++data, end);
        r.token = str_span(start, data);
        r.type = tok_newline;
        return r;
    }

    if (*data == ',') {
        r.data = str_span(++data, end);
        r.token = str_span(start, data);
        r.type = tok_comma;
        return r;
    }

    if (*data == ':') {
        r.data = str_span(++data, end);
        r.token = str_span(start, data);
        r.type = tok_colon;
        return r;
    }

    if (*data == '\'') {
        for (data++; data < end && *data != '\''; data++);
        if (data == end) {
            return r;
        }
        r.data = str_span(&data[1], end);
        r.token = str_span(&start[1], data);
        r.type = tok_string;
        return r;
    }

    if (*data == '-' || *data == '+') {
        for (data++; data < end && is_digit(*data); data++);
        r.data = str_span(data, end);
        r.token = str_span(start, data);
        if (r.token.len < 2) {
            return r;
        }
        r.type = tok_integer;
        return r;
    }

    if (is_digit(*data)) {
        for (data++; data < end && is_digit(*data); data++);
        r.data = str_span(data, end);
        r.token = str_span(start, data);
        r.type = tok_integer;
        return r;
    }

    if (is_letter(*data) || *data == '_') {
        for (data++; data < end && is_identifier(*data); data++);
        r.data = str_span(data, end);
        r.token = str_span(start, data);
        bool is_register = r.token.len == 1 && is_lower(*r.token.data);
        r.type = is_register ? tok_register : tok_identifier;
        return r;
    }

    return r;
}



static insnresult_t parse_instruction(arena_t *a, mnemonic_t m, str_t src)
{
    insnresult_t r = { 0 };
    insn_t *n = alloc(a, insn_t, 1);
    msg_t **tail = &n->head;

    token_t t = { 0 };
    t.data = src;

    int32_t ir = 0;
    switch (m) {
        case m_null:
            return r;
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
                    n->reg[0] = *t.token.data;
                    break;
                default:
                    return r;
            }
            // Eat comma
            t = lex(t.data);
            switch (t.type) {
                case tok_comma:
                    break;
                default:
                    return r;
            }
            // Second operand can be immediate or from reg
            t = lex(t.data);
            switch (t.type) {
                case tok_integer:
                    if (!str_i32(&ir, t.token)) {
                        return r;
                    }
                    n->imm[1] = ir;
                    break;
                case tok_register:
                    n->op++; // change to reg-reg variant
                    n->reg[1] = *t.token.data;
                    break;
                default:
                    return r;
            }
            break;
        case m_cmp:
            // First operand can be imm or reg
            t = lex(t.data);
            switch (t.type) {
                case tok_integer:
                    n->op = op_cmpii;
                    if (!str_i32(&ir, t.token)) {
                        return r;
                    }
                    n->imm[0] = ir;
                    break;
                case tok_register:
                    n->op = op_cmpri;
                    n->reg[0] = *t.token.data;
                    break;
                default:
                    return r;
            }
            t = lex(t.data);
            switch (t.type) {
                case tok_comma:
                    break;
                default:
                    return r;
            }
            // Second operand can be imm or reg
            t = lex(t.data);
            switch (t.type) {
                case tok_integer:
                    if (!str_i32(&ir, t.token)) {
                        return r;
                    }
                    n->imm[1] = ir;
                    break;
                case tok_register:
                    n->op++;
                    n->reg[1] = *t.token.data;
                    break;
                default:
                    return r;
            }
            if (n->op == op_cmpii) {
                return r; // reject "cmp int, int"
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
                return r;
            }
            n->label = t.token;
            n->op = op_jne + (m - m_jne);
            break;
        case m_msg:
            n->op = op_msg;
            for (token_type_t last = tok_error;;) {
                t = lex(t.data);
                switch (t.type) {
                    case tok_newline:
                    case tok_eof:
                        if (last != tok_comma) {
                            r.insn = n;
                            r.src = t.data;
                        }
                        return r;
                    case tok_string:
                        if (last && last != tok_comma) {
                            return r;
                        }
                        *tail = alloc(a, msg_t, 1);
                        (*tail)->string = t.token;
                        tail = &(*tail)->next;
                        break;
                    case tok_register:
                        *tail = alloc(a, msg_t, 1);
                        (*tail)->reg = *t.token.data;
                        tail = &(*tail)->next;
                        break;
                    case tok_comma:
                        if (!last || last == tok_comma) {
                            return r;
                        }
                        break;
                    default:
                        return r;
                }
                last = t.type;
            }
        case m_inc:
        case m_dec:
        case m_neg:
            // One opcode per mnemonic
            n->op = op_incr + (m - m_inc);
            t = lex(t.data);
            switch (t.type) {
                case tok_register:
                    n->reg[0] = *t.token.data;
                    break;
                default:
                    return r;
            }
            break;
        case m_ret:
            n->op = op_ret;
            break;
        case m_end:
            n->op = op_end;
            break;
    }

    t = lex(t.data);
    switch (t.type) {
        case tok_eof:
        case tok_newline:
            r.insn = n;
            r.src = t.data;
            return r;
        default:
            return r;
    }
}

static uintptr_t *upsert(labels_t **t, str_t label, arena_t *a)
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

// Note: returns pointers into the source buffer.
static ast_t parse(str_t src, arena_t *perm, arena_t scratch)
{
    ast_t r = {
        .lineno = 1,
    };

    token_t t = {
        .data = src
    };

    ptrdiff_t addr = 0;
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
            case tok_string:
            case tok_register:
                return r;

            case tok_newline:
                r.lineno++;
                break;

            case tok_eof:
                for (insn_t *n = r.head; n; n = n->next) {
                    if (n->label.data) {
                        uintptr_t *value = upsert(&table, n->label, NULL);
                        if (!value) {
                            r.lineno = n->lineno;
                            return r; // label not found
                        }
                        n->addr = *value;
                    }
                }
                r.ok = true;
                return r;

            case tok_identifier:
                m = parse_mnemonic(t.token);
                if (m != m_null) {
                    insnresult_t ir = parse_instruction(perm, m, t.data);
                    if (!ir.insn) {
                        return r;
                    }
                    t.data = ir.src;
                    ir.insn->lineno = r.lineno++;
                    *tail = ir.insn;
                    tail = &(*tail)->next;
                    addr++;
                }
                else {
                    str_t label = t.token;
                    t = lex(t.data);
                    if (t.type != tok_colon) {
                        return r;
                    }
                    *upsert(&table, label, &scratch) = addr;
                }
                break;
        }
    }
}

// Note: retains references to the AST.
static psw_t *assemble(insn_t *head, arena_t *perm)
{
    ptrdiff_t len = 0;
    for (insn_t *n = head; n; n = n->next) {
        len++;
    }

    ptrdiff_t i = 0;
    psw_t *program = alloc(perm, psw_t, len + 1);
    for (insn_t *n = head; n; n = n->next, i++) {
        program[i].op = (uint8_t)n->op;
        switch (n->op) {
            case op_abort:
            case op_cmpii:
                return NULL;
            case op_ret:
            case op_end:
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
    }

    return program;
}

static result_t execute(psw_t *program, arena_t a)
{
    result_t r = { 0 };

    r.out.cap = 1 << 16;
    r.out.data = alloc(&a, uint8_t, r.out.cap);

    ptrdiff_t len = 0;
    ptrdiff_t cap = 1 << 10;
    ptrdiff_t *stack = alloc(&a, uint8_t, cap);

    int32_t cmp = 0;
    int32_t regs[26] = { 0 };

    for (ptrdiff_t i = 0;; i++) {
        int32_t a = 0;
        int32_t b = 0;
        psw_t *w = &program[i];
        switch (w->op) {
            case op_abort:
                return r;
#pragma region INC/DEC/NEG
            case op_incr:
                regs[w->reg[0] - 'a'] += (uint32_t)1;
                break;
            case op_decr:
                regs[w->reg[0] - 'a'] -= (uint32_t)1;
                break;
            case op_negr:
                regs[w->reg[0] - 'a'] = -regs[w->reg[0] - 'a'];
                break;
#pragma endregion
#pragma region MOV
            case op_movri:
                regs[w->reg[0] - 'a'] = w->operand.imm;
                break;
            case op_movrr:
                regs[w->reg[0] - 'a'] = regs[w->reg[1] - 'a'];
                break;
#pragma endregion
#pragma region ADD
            case op_addri:
                regs[w->reg[0] - 'a'] += (uint32_t)w->operand.imm;
                break;
            case op_addrr:
                regs[w->reg[0] - 'a'] += (uint32_t)regs[w->reg[1] - 'a'];
                break;
#pragma endregion
#pragma region SADD
            case op_saddri:
                regs[w->reg[0] - 'a'] += w->operand.imm;
                break;
            case op_saddrr:
                regs[w->reg[0] - 'a'] += regs[w->reg[1] - 'a'];
                break;
#pragma endregion
#pragma region SUB
            case op_subri:
                regs[w->reg[0] - 'a'] -= (uint32_t)w->operand.imm;
                break;
            case op_subrr:
                regs[w->reg[0] - 'a'] -= (uint32_t)regs[w->reg[1] - 'a'];
                break;
#pragma endregion
#pragma region MUL
            case op_mulri:
                regs[w->reg[0] - 'a'] *= (uint32_t)w->operand.imm;
                break;
            case op_mulrr:
                regs[w->reg[0] - 'a'] *= (uint32_t)regs[w->reg[1] - 'a'];
                break;
#pragma endregion
#pragma region DIV
            case op_divri:
                switch (w->operand.imm) {
                    case 0:
                        return r;
                    case -1:
                        regs[w->reg[0] - 'a'] = -(uint32_t)regs[w->reg[0] - 'a'];
                        break;
                    default:
                        regs[w->reg[0] - 'a'] /= (uint32_t)w->operand.imm;
                }
                break;
            case op_divrr:
                switch (regs[w->reg[1] - 'a']) {
                    case 0:
                        return r;
                    case -1:
                        regs[w->reg[0] - 'a'] = -(uint32_t)regs[w->reg[0] - 'a'];
                        break;
                    default:
                        regs[w->reg[0] - 'a'] /= (uint32_t)regs[w->reg[1] - 'a'];
                }
                break;
#pragma endregion
#pragma region SDIV
            case op_sdivri:
                switch (w->operand.imm) {
                    case 0:
                        return r;
                    case -1:
                        regs[w->reg[0] - 'a'] = -(uint32_t)regs[w->reg[0] - 'a'];
                        break;
                    default:
                        regs[w->reg[0] - 'a'] /= w->operand.imm;
                }
                break;
            case op_sdivrr:
                switch (regs[w->reg[1] - 'a']) {
                    case 0:
                        return r;
                    case -1:
                        regs[w->reg[0] - 'a'] = -(uint32_t)regs[w->reg[0] - 'a'];
                        break;
                    default:
                        regs[w->reg[0] - 'a'] /= regs[w->reg[1] - 'a'];
                }
                break;
#pragma endregion
#pragma region MOD
            case op_modri:
                switch (w->operand.imm) {
                    case 0:
                        break;
                    default:
                        regs[w->reg[0] - 'a'] %= (uint32_t)w->operand.imm;
                }
                break;
            case op_modrr:
                switch (regs[w->reg[1] - 'a']) {
                    case 0:
                        break;
                    default:
                        regs[w->reg[0] - 'a'] %= (uint32_t)regs[w->reg[1] - 'a'];
                }
                break;
#pragma endregion
#pragma region SMOD
            case op_smodri:
                switch (w->operand.imm) {
                    case 0:
                        break;
                    case -1:
                        if (regs[w->reg[0] - 'a'] == INT32_MIN) {
                            regs[w->reg[0] - 'a'] = 0;
                            break;
                        }
                    default:
                        regs[w->reg[0] - 'a'] %= w->operand.imm;
                }
                break;
            case op_smodrr:
                switch (regs[w->reg[1] - 'a']) {
                    case 0:
                        break;
                    case -1:
                        if (regs[w->reg[0] - 'a'] == INT32_MIN) {
                            regs[w->reg[0] - 'a'] = 0;
                            break;
                        }
                    default:
                        regs[w->reg[0] - 'a'] %= regs[w->reg[1] - 'a'];
                }
                break;
#pragma endregion
#pragma region AND
            case op_andri:
                regs[w->reg[0] - 'a'] = (uint32_t)regs[w->reg[0] - 'a'] & (uint32_t)w->operand.imm;
                break;
            case op_andrr:
                regs[w->reg[0] - 'a'] = (uint32_t)regs[w->reg[0] - 'a'] & (uint32_t)regs[w->reg[1] - 'a'];
                break;
#pragma endregion
#pragma region OR
            case op_orri:
                regs[w->reg[0] - 'a'] = (uint32_t)regs[w->reg[0] - 'a'] | (uint32_t)w->operand.imm;
                break;
            case op_orrr:
                regs[w->reg[0] - 'a'] = (uint32_t)regs[w->reg[0] - 'a'] | (uint32_t)regs[w->reg[1] - 'a'];
                break;
#pragma endregion
#pragma region XOR
            case op_xorri:
                regs[w->reg[0] - 'a'] = (uint32_t)regs[w->reg[0] - 'a'] ^ (uint32_t)w->operand.imm;
                break;

            case op_xorrr:
                regs[w->reg[0] - 'a'] = (uint32_t)regs[w->reg[0] - 'a'] ^ (uint32_t)regs[w->reg[1] - 'a'];
                break;
#pragma endregion
#pragma region LSL
            case op_lslri:
                regs[w->reg[0] - 'a'] <<= (uint32_t)w->operand.imm;
                break;
            case op_lslrr:
                regs[w->reg[0] - 'a'] <<= (uint32_t)regs[w->reg[1] - 'a'];
                break;
#pragma endregion
#pragma region LSR
            case op_lsrri:
                regs[w->reg[0] - 'a'] = (uint32_t)regs[w->reg[0] - 'a'] >> (uint32_t)w->operand.imm;
                break;
            case op_lsrrr:
                regs[w->reg[0] - 'a'] = (uint32_t)regs[w->reg[0] - 'a'] >> (uint32_t)regs[w->reg[1] - 'a'];
                break;
#pragma endregion
#pragma region ASR
            case op_asrri:
                regs[w->reg[0] - 'a'] >>= (uint32_t)w->operand.imm;
                break;
            case op_asrrr:
                regs[w->reg[0] - 'a'] >>= (uint32_t)regs[w->reg[1] - 'a'];
                break;
#pragma endregion
#pragma region CMP
            case op_cmpii:
                return r;
            case op_cmpir:
                a = w->operand.imm;
                b = regs[w->reg[1] - 'a'];
                cmp = (a > b) - (a < b);
                break;
            case op_cmpri:
                a = regs[w->reg[0] - 'a'];
                b = w->operand.imm;
                cmp = (a > b) - (a < b);
                break;
            case op_cmprr:
                a = regs[w->reg[0] - 'a'];
                b = regs[w->reg[1] - 'a'];
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
                    return r; // stack overflow
                }
                stack[len++] = i;
                i = w->operand.addr - 1;
                break;
#pragma endregion
#pragma region RET
            case op_ret:
                if (!len) {
                    return r; // stack empty
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
                        print_i32(&r.out, regs[m->reg - 'a']);
                    }
                }
                break;
#pragma endregion
#pragma region END
            case op_end:
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

static status_t run(arena_t heap)
{
    status_t r = { 0 };

    arena_t a = { 0 };
    ptrdiff_t cap = (heap.end - heap.start) / 2;

    a.start = alloc(&heap, uint8_t, cap);
    a.end = &a.start[cap];

    output_t std_err = { 0 };
    std_err.cap = 1 << 8;
    std_err.data = alloc(&heap, uint8_t, std_err.cap);

    str_t src = os_loadstdin(&heap);

    ast_t ast = parse(src, &heap, a);
    if (!ast.ok) {
        print_str(&std_err, S("[error] invalid line:"));
        print_i32(&std_err, (int32_t)ast.lineno);
        print_str(&std_err, S("\n"));
        r.err = (str_t) {
            .data = std_err.data,
            .len = std_err.len,
        };
        r.status = 2;
        return r;
    }

    psw_t *program = assemble(ast.head, &heap);
    result_t er = execute(program, a);
    if (!er.ok) {
        r.err = S("[error] execution aborted\n");
        r.status = 1;
        return r;
    }
    r.out = (str_t) {
        .data = er.out.data,
        .len = er.out.len,
    };
    return r;
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
    status_t r = run(heap);
    if (r.err.len) {
        fwrite(r.err.data, 1, r.err.len, stderr);
    }
    if (r.out.len) {
        fwrite(r.out.data, 1, r.out.len, stdout);
    }
    fflush(stdout);
    return ferror(stdout) ? 100 : 0;
}
