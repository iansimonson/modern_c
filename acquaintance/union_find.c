#include <assert.h>
#include <stdio.h>
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

void *xmalloc(size_t length) {
    void *ptr = malloc(length);
    assert(ptr != NULL);
    return ptr;
}

void xfree(void *ptr, size_t len) {
    free(ptr);
}

#define ROOT_NODE SIZE_MAX

typedef struct dj_set_t {
    size_t *data;
    size_t len;
} DJSet;

size_t djset_find(DJSet set, size_t node) {
    assert(node < set.len);
    size_t pnode = node;
    size_t parent = set.data[node];
    while (parent != ROOT_NODE) {
        pnode = parent;
        parent = set.data[parent];
    }
    return pnode;
}

size_t djset_find_replace(DJSet set[static 1], size_t node, size_t replace) {
    for (size_t parent = node; parent != ROOT_NODE;) {
        size_t next = set->data[parent];
        set->data[parent] = replace;
        parent = next;
    }
    return replace;
}

size_t djset_find_compress(DJSet set[static 1], size_t node) {
    size_t pnode = djset_find(*set, node);
    for (size_t parent = node; parent != pnode;) {
        size_t next = set->data[parent];
        set->data[parent] = pnode;
        parent = next;
    }
    return pnode;
}

void djset_union(DJSet set[static 1], size_t n1, size_t n2) {
    size_t r1 = djset_find_compress(set, n1);
    djset_find_replace(set, n2, r1);
}

DJSet dj_set_make(size_t size) {
    DJSet r = {};
    r.data = xmalloc(size * sizeof(size_t));
    r.len = size;
    return r;
}

int main() {
    DJSet set = dj_set_make(100);
    set.data[0] = ROOT_NODE;
    set.data[1] = 0;
    set.data[2] = 1;
    set.data[3] = 0;
    set.data[4] = 2;

    set.data[5] = ROOT_NODE;
    set.data[6] = ROOT_NODE;
    set.data[7] = 5;
    set.data[8] = 6;
    set.data[9] = 8;

    size_t r4 = djset_find(set, 4);
    assert(r4 == 0);

    djset_find_compress(&set, 4);
    assert(set.data[2] == 0);

    djset_union(&set, 4, 9);
    size_t r9 = djset_find(set, 9);
    assert(r9 == 0);

    printf("DONE\n");
}
