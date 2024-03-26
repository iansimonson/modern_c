#pragma once

#include <assert.h>
#include <stdlib.h>

#define swap(a, b) do { \
    typeof((a)) tmp = (a); \
    (a) = (b); \
    (b) = (tmp); \
} while(0)

#define copy(dest, src, len) do { \
    memcpy((dest), (src), sizeof((src)[0]) * len); \
} while(0)

#define array_count(arr) sizeof((arr)) / sizeof((arr)[0])

inline static void *xmalloc(size_t length) {
    void *ptr = malloc(length);
    assert(ptr != NULL);
    return ptr;
}

inline static void xfree(void *ptr, size_t len) {
    free(ptr);
}
