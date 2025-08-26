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

// Create a string spanning [head, tail)
str_t str_span(uint8_t *head, uint8_t *tail);

// Create a string from [head, head + len)
str_t str_from(uint8_t *head, int64_t len);

// Case sensitive string compare
bool str_equal(str_t s1, str_t s2);

// Case insensitive string compare
bool str_equali(str_t s1, str_t s2);

uint64_t str_hash(str_t s);

// Convert `s` to an integer
bool str_i32(int32_t *d, str_t s);

// Convert an integer `d` to string
str_t i32_str(int32_t d);

// Check for `c` in (' ', '\t', '\r')
bool is_whitespace(uint8_t c);

// Check for `c` in '0'..'9'
bool is_digit(uint8_t c);

// Check for `c` in 'A'..'Z'
bool is_upper(uint8_t c);

// Convert `c` to uppercase if it's in 'a'..'z'
uint8_t to_upper(uint8_t c);

// Check for `c` in 'a'..'z'
bool is_lower(uint8_t c);

// Convert `c` to lowercase if it's in 'A'..'Z'
uint8_t to_lower(uint8_t c);

// Convert `s` to lowercase
str_t str_lower(str_t s);

// Check for `c` in 'A'..'Z' or 'a'..'z'
bool is_letter(uint8_t c);

bool is_identifier(uint8_t c);

str_t str_trim(str_t s);

str_t str_slice(str_t s, ptrdiff_t start, ptrdiff_t end);

// Check if `s` starts with `prefix`
bool str_has_prefix(str_t s, str_t prefix);

// Check if `s` ends with `suffix`
bool str_has_suffix(str_t s, str_t suffix);

// Check if `c` in '0'..'9', 'a'..'f', 'A'..'F'
bool is_hex(uint8_t c);

int32_t from_hex(uint8_t c);

splitstr_t str_chop(str_t s);

// Decode any escape characters in-place
str_t str_quoted_span(uint8_t *head, uint8_t *tail);
