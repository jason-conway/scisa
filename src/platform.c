#include "platform.h"

int32_t os_read(int32_t fd, uint8_t *d, size_t len)
{
    ptrdiff_t r = read(fd, d, len);
    if (r < 1) {
        return 0;
    }
    return r;
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

str_t os_loadfile(arena_t *a, const char *filename)
{
    str_t s = { 0 };

    struct stat st;
    if (stat(filename, &st) < 0) {
        return s;
    }

    int64_t len = st.st_size;
    if (len < 1) {
        return s;
    }
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "[error] unable to open \"%s\"\n", filename);
        return s;
    }
    s.data = alloc(a, uint8_t, len);
    s.len = fread(s.data, 1, len, fp);
    if (s.len != len) {
        fprintf(stderr, "[error] unable to read entire file\n");
    }
    (void)fclose(fp);
    return s;
}
