#include "scisa.h"

str_t str_span(uint8_t *head, uint8_t *tail)
{
    return (str_t) {
        .data = head,
        .len = tail - head
    };
}

bool str_equal(str_t s1, str_t s2)
{
    if (s1.len != s2.len) {
        return false;
    }
    for (int64_t i = 0; i < s1.len; i++) {
        if (s1.data[i] != s2.data[i]) {
            return false;
        }
    }
    return true;
}

uint64_t str_hash(str_t s)
{
    uint64_t h = UINT64_C(0xcbf29ce484222325);
    for (int64_t i = 0; i < s.len; i++) {
        h ^= s.data[i];
        h *= UINT64_C(0x100000001b3);
    }
    return h ^ h >> 32;
}

// Convert s8 ascii string to int32_t
// Return false on failure
bool str_i32(int32_t *d, str_t s)
{
    size_t i = 0;
    bool neg = false;
    uint32_t value = 0;
    uint32_t limit = 0x7fffffff;

    switch (*s.data) {
        case '-':
            i = 1;
            neg = true;
            limit = 0x80000000;
            break;
        case '+':
            i = 1;
            break;
    }

    for (; i < (size_t)s.len; i++) {
        int32_t d = s.data[i] - '0';
        if (value > (limit - d) / 10) {
            return false;
        }
        value = value * 10 + d;
    }

    *d = neg ? -value : value;
    return true;
}

str_t i32_str(int32_t v)
{
    static uint8_t data[16] = { 0 };
    uint8_t *end = &data[countof(data)];
    uint8_t *start = end;
    int32_t i = v < 0 ? v : -v;
    do {
        *--start = '0' - (uint8_t)(i % 10);
    } while (i /= 10);
    if (v < 0) {
        *--start = '-';
    }
    return str_span(start, end);
}

bool is_whitespace(uint8_t c)
{
    return c == '\t' || c == '\r' || c == ' ';
}

bool is_digit(uint8_t c)
{
    return c >= '0' && c <= '9';
}

bool is_upper(uint8_t c)
{
    return c >= 'A' && c <= 'Z';
}

uint8_t to_upper(uint8_t c)
{
    return is_upper(c) ? c : (uint8_t)(c + 'A' - 'a');
}

bool is_lower(uint8_t c)
{
    return c >= 'a' && c <= 'z';
}

uint8_t to_lower(uint8_t c)
{
    return is_lower(c) ? c : (uint8_t)(c + 'a' - 'A');
}

bool is_letter(uint8_t c)
{
    return is_upper(c) || is_lower(c);
}

bool is_identifier(uint8_t c)
{
    return c == '_' || is_letter(c) || is_digit(c);
}

bool is_register(str_t s)
{
    const int64_t len = s.len;
    if (!len || len > 3) {
        return false;
    }

    uint8_t *c = &s.data[0];
    if (to_lower(c[0]) != 'r') {
        return false;
    }
    for (int64_t i = 1; i < s.len; i++) {
        if (!is_digit(c[i])) {
            return false;
        }
    }
    return true;
}

str_t str_trim(str_t s)
{
    if (!s.len) {
        return s;
    }
    uint8_t *data = s.data;
    uint8_t *end = &data[s.len];
    for (; data < end && is_whitespace(*data); data++);
    s.data = data;
    s.len = end - data;
    return s;
}
