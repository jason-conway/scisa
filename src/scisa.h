#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "isa.h"
#include "std.h"
#include "str.h"

#define countof(a)     (sizeof(a) / sizeof(*(a)))
#define lengthof(s)    (countof(s) - 1)
#define alloc(a, t, n) __alloc(a, sizeof(t), _Alignof(t), n)

#define GEN_MNEMONIC_ID(m) m_##m
#define GEN_OPCODE_ID(o)   op_##o
#define GEN_STR(x)         E(STR(x))


typedef enum opcode_t {
    MAP(GEN_OPCODE_ID, OPCODE_LIST)
} opcode_t;

typedef enum reg_t {
    REGISTER_LIST,
    r_max,
} reg_t;

typedef enum mnemonic_t {
    m_null,
    MAP(GEN_MNEMONIC_ID, MNEMONICS_LIST)
} mnemonic_t;

typedef uint32_t vaddr;

typedef enum tok_t tok_t;

typedef struct token_t {
    enum tok_t {
        tok_error,
        tok_eof,
        tok_newline,
        tok_comma,
        tok_colon,
        tok_integer,
        tok_string,
        tok_register,
        tok_identifier,
    } type;
    str_t data;
    str_t token;
} token_t;

typedef struct msg_t msg_t;
struct msg_t {
    msg_t *next;
    str_t string;
    uint8_t reg;
};

typedef struct insn_t insn_t;
struct insn_t {
    insn_t *next;
    msg_t *head;
    str_t label;
    vaddr addr;
    size_t lineno;
    opcode_t op;
    int32_t imm[2];
    uint8_t reg[2];
};

typedef struct ast_t {
    insn_t *head;
    size_t lineno;
    bool ok;
} ast_t;

typedef struct labels_t labels_t;
struct labels_t {
    labels_t *child[4];
    str_t label;
    vaddr addr;
};

typedef struct insnresult_t {
    str_t data;
    insn_t *insn;
} insnresult_t;

// program status word
typedef struct psw_t {
    uint8_t op;
    uint8_t reg[2];
    union operand_t {
        int32_t imm;
        vaddr addr;
        msg_t *head;
    } operand;
} psw_t;

typedef struct output_t {
    uint8_t *data;
    ptrdiff_t len;
    ptrdiff_t cap;
    int32_t fd;
    bool err;
} output_t;

typedef struct result_t {
    output_t out;
    bool ok;
} result_t;

typedef struct arena_t {
    uint8_t *start;
    uint8_t *end;
} arena_t;
