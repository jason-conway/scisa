#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "str.h"

#define alignof(x)   _Alignof(x)
#define countof(a)   (sizeof(a) / sizeof(*(a)))
#define lengthof(s)  (countof(s) - 1)
#define alloc(a, t, n) __alloc(a, sizeof(t), alignof(t), n)

#if __GNUC__
#define __malloc __attribute__((malloc, alloc_size(2, 4)))
#endif

typedef enum token_type_t {
    tok_error,
    tok_eof,
    tok_newline,
    tok_comma,
    tok_colon,
    tok_integer,
    tok_string,
    tok_register,
    tok_identifier,
} token_type_t;

typedef struct token_t {
    token_type_t type;
    str_t data;
    str_t token;
} token_t;

typedef struct msg_t msg_t;
struct msg_t {
    msg_t *next;
    str_t string;
    uint8_t reg;
};


