#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct arena_t {
    uint8_t *start;
    uint8_t *end;
} arena_t;

void *__alloc(arena_t *a, size_t objsize, size_t align, size_t count);

#ifndef alloc
    #define alloc(a, t, n) __alloc(a, sizeof(t), _Alignof(t), n)
#endif
