#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "isa.h"
#include "std.h"
#include "str.h"
#include "io.h"
#include "arena.h"

#ifndef countof
    #define countof(a)     (sizeof(a) / sizeof(*(a)))
#endif
#ifndef lengthof
    #define lengthof(s)    (countof(s) - 1)
#endif

#define GEN_MNEMONIC_ID(m)  m_##m
#define GEN_OPCODE_ID(o)    op_##o
#define GEN_DIRECTIVE_ID(o) dir_##o
#define GEN_STR(x)          E(STR(x))

typedef enum seg_t {
    seg_null,
    seg_text,
    seg_data,
    seg_max = seg_data,
} seg_t;

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

typedef enum directive_t {
    dir_null,
    MAP(GEN_DIRECTIVE_ID, DIRECTIVE_LIST)
} directive_t;

typedef uint32_t vaddr;

typedef uint64_t sym_addr_t;

typedef enum tok_t tok_t;

enum scisa {
    DEFAULT_ALIGNMENT = sizeof(vaddr)
};

typedef struct token_t {
    enum tok_t {
        tok_error,
        tok_eof,
        tok_newline,
        tok_comma,
        tok_colon,
        tok_lparen,
        tok_rparen,
        tok_integer,
        tok_string,
        tok_register,
        tok_identifier,
        tok_directive,
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

typedef struct data_t data_t;
struct data_t {
    data_t *next;
    uint32_t lineno;
    uintptr_t data;
    uint16_t sz;
    uint8_t align;
    bool is_ascii;
    bool is_zero;
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
    struct seg_heads_t {
        insn_t *insn;
        data_t *data;
    } head;
    size_t lineno;
    bool ok;
} ast_t;

typedef struct labels_t labels_t;
struct labels_t {
    labels_t *child[4];
    str_t label;
    sym_addr_t addr;
};

// Sweetened Condensed Immediate Representation (SCIR)
typedef struct scir_t {
    uint8_t op;
    uint8_t reg[2];
    struct operand_t {
        int32_t imm[2];
        vaddr addr;
        msg_t *head;
    } operand;
} scir_t;

typedef struct memory_region_t {
    uint8_t *base;
    uint32_t size;
} memory_region_t;


typedef struct seg_addrs_t {
    vaddr insn;
    vaddr data;
} seg_addrs_t;

// Sweetened Condensed Object File Format (SCOFF)
typedef struct scoff_t {
    scir_t *code;
    memory_region_t data;
    memory_region_t stack;
    bool ok;
} scoff_t;

typedef struct result_t {
    output_t out;
    bool ok;
} result_t;

typedef enum cc_t {
    CC_NULL,
    CC_NE = 1u << 0,
    CC_EQ = 1u << 1,
    CC_GE = 1u << 2, // signed GE
    CC_GT = 1u << 3, // signed GT
    CC_LE = 1u << 4, // signed LE
    CC_LT = 1u << 5, // signed LT
    CC_HS = 1u << 6, // unsigned GE
    CC_HI = 1u << 7, // unsigned GT
    CC_LS = 1u << 8, // unsigned LE
    CC_LO = 1u << 9, // unsigned LT
} cc_t;



result_t execute(scoff_t obj, arena_t arena);
