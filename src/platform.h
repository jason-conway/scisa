#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include "std.h"
#include "str.h"
#include "arena.h"

// Read up to `len` bytes from `fd` into `d`, returning the actual number of bytes read
int32_t os_read(int32_t fd, uint8_t *d, size_t len);

// Write `len` bytes from `s` to `fd`
bool os_write(int32_t fd, uint8_t *s, size_t len);

// Load an entire file into an string allocated out of `a`
str_t os_loadfile(arena_t *a, const char *filename);
