#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#include "std.h"
#include "str.h"
#include "arena.h"

// Read `len` bytes from `fd` into `d`
bool os_read(int32_t fd, uint8_t *d, size_t len);

// Write `len` bytes from `s` to `fd`
bool os_write(int32_t fd, uint8_t *s, size_t len);

// Load all data from stdin into an allocated string in arena `a`
str_t os_loadstdin(arena_t *a);
