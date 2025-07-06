#include "io.h"

static bool full_write(int32_t fd, uint8_t *s, size_t len)
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

static void flush_output(output_t *o)
{
    if (!o->err && o->len) {
        o->err = !full_write(o->fd, o->data, o->len);
        o->len = 0;
    }
}

void print_str(output_t *o, str_t s)
{
    uint8_t *c = &s.data[0];
    uint8_t *end = &s.data[s.len];
    while (c < end && !o->err) {
        ptrdiff_t avail = o->cap - o->len;
        uint8_t *dst = &o->data[o->len];

        ptrdiff_t i = 0;
        ptrdiff_t j = 0;

        while (i < end - c && j < avail) {
            uint8_t e = c[i];
            if (e == '\\' && i + 1 < end - c) {
                switch (c[i + 1]) {
                    case 't':
                        e = '\t';
                        break;
                    case 'n':
                        e = '\n';
                        break;
                    case 'r':
                        e = '\r';
                        break;
                    case '\\':
                        e = '\\';
                        break;
                    case 'e':
                        e = '\e';
                        break;
                    case '0':
                        flush_output(o);
                        i += 2;
                        c += i;
                        goto next;
                    default:
                        __builtin_trap();
                }
                i++;
            }
            dst[j++] = e;
            i++;
        }

        c += i;
        o->len += j;
        if (o->len == o->cap) {
            flush_output(o);
        }
next:;
    }
}
