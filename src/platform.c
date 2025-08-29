#include "platform.h"

bool os_read(int32_t fd, uint8_t *d, size_t len)
{
    for (size_t off = 0; off < len;) {
        ptrdiff_t r = read(fd, &d[off], len - off);
        if (r < 1) {
            return false;
        }
        off += r;
    }
    return true;
}

bool os_write(int32_t fd, uint8_t *s, size_t len)
{
    for (size_t off = 0; off < len;) {
        ptrdiff_t r = write(fd, &s[off], len - off);
        if (r < 1) {
            return false;
        }
        off += r;
    }
    return true;
}
str_t os_loadstdin(arena_t *a)
{
    str_t s = { 0 };
    bool err = false;

    err |= fseek(stdin, 0, SEEK_END);
    size_t len = ftell(stdin);
    err |= len < 1;
    err |= fseek(stdin, 0, SEEK_SET);
    if (err) {
        return s;
    }

    s.data = alloc(a, uint8_t, len);
    s.len = fread(s.data, 1, len, stdin);
    return s;
}
