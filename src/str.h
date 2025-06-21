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

#if __GNUC__
#define __malloc __attribute__((malloc, alloc_size(2, 4)))
#endif

typedef struct str_t {
    uint8_t *data;
    ptrdiff_t len;
} str_t;

#define S(s) (str_t) { (uint8_t *)s, lengthof(s) }

#define E(s) { (uint8_t *)s, lengthof(s) }

typedef struct arena_t {
    uint8_t *start;
    uint8_t *end;
} arena_t;

str_t str_span(uint8_t *head, uint8_t *tail);

bool str_equal(str_t s1, str_t s2);

uint64_t str_hash(str_t s);

bool str_i32(int32_t *d, str_t s);

bool is_whitespace(uint8_t c);

bool is_digit(uint8_t c);

bool is_upper(uint8_t c);
uint8_t to_upper(uint8_t c);

bool is_lower(uint8_t c);

bool is_letter(uint8_t c);
bool is_register(str_t s);

bool is_invalid_identifier(uint8_t c);

bool is_identifier(uint8_t c);

str_t str_trim(str_t s);
