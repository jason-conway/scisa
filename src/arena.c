#include "arena.h"

void *__alloc(arena_t *a, size_t objsize, size_t align, size_t count)
{
    ptrdiff_t avail = a->end - a->start;
    ptrdiff_t padding = -(uintptr_t)a->start & (align - 1);
    if (count > (avail - padding) / objsize) {
        trap("oom");
    }
    size_t total = objsize * count;

    uint8_t *r = &a->start[padding];
    for (size_t i = 0; i < total; i++) {
        a->start[padding + i] = 0;
    }
    a->start += padding + total;
    return r;
}
