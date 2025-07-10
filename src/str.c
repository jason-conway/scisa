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

int32_t from_hex(uint8_t c)
{
    static uint8_t vals[256] = {
        ['0'] = 0x01, ['1'] = 0x02,
        ['2'] = 0x03, ['3'] = 0x04,
        ['4'] = 0x05, ['5'] = 0x06,
        ['6'] = 0x07, ['7'] = 0x08,
        ['8'] = 0x09, ['9'] = 0x0a,
        ['A'] = 0x0b, ['B'] = 0x0c,
        ['C'] = 0x0d, ['D'] = 0x0e,
        ['E'] = 0x0f, ['F'] = 0x10,
        ['a'] = 0x0b, ['b'] = 0x0c,
        ['c'] = 0x0d, ['d'] = 0x0e,
        ['e'] = 0x0f, ['f'] = 0x10,
    };
    return vals[c] - 1;
}

bool is_hex(uint8_t c)
{
    return !(from_hex(c) < 0);
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

    if (str_has_prefix(str_slice(s, i, s.len), S("0x"))) {
        for (i += 2; i < (size_t)s.len; i++) {
            int32_t v = from_hex(s.data[i]);
            if (v < 0) {
                return false;
            }
            if (value > (limit - v) / 16) {
                return false;
            }
            value *= 16;
            value += v;
        }
    }
    else {
        for (; i < (size_t)s.len; i++) {
            int32_t v = s.data[i] - '0';
            if (value > (limit - v) / 10) {
                return false;
            }
            value *= 10;
            value += v;
        }
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

str_t str_lower(str_t s)
{
    for (int64_t i = 0; i < s.len; i++) {
        if (is_letter(s.data[i])) {
            s.data[i] = to_lower(s.data[i]);
        }
    }
    return s;
}

str_t str_upper(str_t s)
{
    for (int64_t i = 0; i < s.len; i++) {
        if (is_letter(s.data[i])) {
            s.data[i] = to_upper(s.data[i]);
        }
    }
    return s;
}

bool is_letter(uint8_t c)
{
    return is_upper(c) || is_lower(c);
}

bool is_identifier(uint8_t c)
{
    return c == '_' || is_letter(c) || is_digit(c);
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

str_t str_slice(str_t s, ptrdiff_t start, ptrdiff_t end)
{
    s.data = &s.data[start];
    s.len = end - start;
    return s;
}

bool str_has_prefix(str_t s, str_t prefix)
{
    if (s.len < prefix.len) {
        return false;
    }
    return str_equal(str_slice(s, 0, prefix.len), prefix);
}

bool str_has_suffix(str_t s, str_t suffix)
{
    if (s.len < suffix.len) {
        return false;
    }
    return str_equal(str_slice(s, s.len - suffix.len, s.len), suffix);
}

splitstr_t str_cut(str_t s, uint8_t c)
{
    uint8_t *start = &s.data[0];
    uint8_t *end = &s.data[s.len];
    uint8_t *cut = start;

    for (; cut < end && *cut != c; cut++);

    splitstr_t r = {};
    r.ok = cut < end;
    r.head = str_span(start, cut);
    r.tail = str_span(cut + r.ok, end);
    return r;
}

splitstr_t str_chop(str_t s)
{
    splitstr_t r = { 0 };
    if (str_has_suffix(s, S("\r\n"))) {
        r.ok = true;
        r.head = str_slice(s, 0, s.len - 2);
        r.tail = str_slice(s, s.len - 2, s.len);
        return r;
    } else if (str_has_suffix(s, S("\n"))) {
        r.ok = true;
        r.head = str_slice(s, 0, s.len - 1);
        r.tail = str_slice(s, s.len - 1, s.len);
        return r;
    }
    r.head = s;
    return r;
}
