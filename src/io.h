#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "str.h"

typedef struct output_t {
    uint8_t *data;
    ptrdiff_t len;
    ptrdiff_t cap;
    int32_t fd;
    bool err;
} output_t;

void print_str(output_t *o, str_t s);
void flush_output(output_t *o);
void xhexdump(const void *src, size_t len);
