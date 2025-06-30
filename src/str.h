#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef countof
    #define countof(a)   (sizeof(a) / sizeof(*(a)))
#endif
#ifndef lengthof
    #define lengthof(s)  (countof(s) - 1)
#endif

typedef struct str_t {
    uint8_t *data;
    int64_t len;
} str_t;

typedef struct splitstr_t {
    str_t head;
    str_t tail;
    bool ok;
} splitstr_t;


#define S(s) (str_t) { (uint8_t *)s, lengthof(s) }

#define E(s) { (uint8_t *)s, lengthof(s) }

str_t str_span(uint8_t *head, uint8_t *tail);

bool str_equal(str_t s1, str_t s2);

uint64_t str_hash(str_t s);

bool str_i32(int32_t *d, str_t s);

str_t i32_str(int32_t v);

bool is_whitespace(uint8_t c);

bool is_digit(uint8_t c);

bool is_upper(uint8_t c);

uint8_t to_upper(uint8_t c);

bool is_lower(uint8_t c);

uint8_t to_lower(uint8_t c);

str_t str_lower(str_t s);

bool is_letter(uint8_t c);

bool is_register(str_t s);

bool is_identifier(uint8_t c);

str_t str_trim(str_t s);

str_t str_slice(str_t s, ptrdiff_t start, ptrdiff_t end);

bool str_has_prefix(str_t s, str_t prefix);

bool str_has_suffix(str_t s, str_t suffix);

bool is_hex(uint8_t c);

int32_t from_hex(uint8_t c);

splitstr_t str_chop(str_t s);
