#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "mcstd.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

typedef struct {
    size_t num_pixels;
    size_t sum;
} Stat;

void usage(char name[static 1]);
size_t uf_find(size_t parents[static 1], size_t element);
size_t uf_find_replace(size_t parents[static 1], size_t element, size_t replace);
size_t uf_find_compress(size_t parents[static 1], size_t element);
void uf_union(size_t parents[static 1], size_t element_1, size_t element_2);
void calculate_stats(size_t size, u8 image[static 1], size_t parents[static 1], Stat stats[static 1]);
void print_stats(Stat stats[static 1], size_t size);
bool can_merge(size_t image_size, Stat stats[static 1], size_t root_1, size_t root_2);

enum STBI_REQ_IMAGE {
    Auto = 0,
    Grey,
    Grey_Alpha,
    RGB,
    RGBA,
};

int main(int argc, char *argv[argc]) {

    if (argc != 2) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    FILE *image = fopen(argv[1], "r");
    MCS_ASSERT(image != NULL, "fopen failed");
    int image_handle = fileno(image);
    struct stat buf = {};
    MCS_ASSERT(fstat(image_handle, &buf) == 0, "fstat failed");

    int x, y, n;
    u8 *data = stbi_load_from_file(image, &x, &y, &n, Grey);
    if (data == NULL) {
        mcs_eprintfln("Could not load image data from %s", argv[1]);
        usage(argv[0]);
    }

    printf("image filesize: %lld KiB\n", buf.st_size / 1024);
    printf("IMAGE LOADED: (%d x %d)\n", x, y);

    size_t image_size = x * y;
    size_t *image_roots = mcs_make_array(image_size, sizeof(size_t));
    for (size_t i = 0; i < image_size; ++i) {
        image_roots[i] = SIZE_MAX;
    }

    Stat *image_stats = mcs_make_array(x * y, sizeof(Stat));
    calculate_stats(image_size, data, image_roots, image_stats);

    for (bool changed = true; changed; changed = false) {
        for (int row = 0; row < y; ++row) {
            for (int col = 0; col < x; ++col) {
                size_t idx = row * x + col;
                size_t my_root = uf_find_compress(image_roots, idx);
                if (col - 1 >= 0) {
                    size_t rl = uf_find_compress(image_roots, idx - 1);
                    if (rl != my_root && can_merge(image_size, image_stats, my_root, rl)) {
                        // printf("MERGING %zu and %zu - %zu, %zu\n", rl, my_root, image_stats[rl].sum, image_stats[rl].num_pixels);
                        uf_union(image_roots, rl, my_root);
                        image_stats[rl].num_pixels += image_stats[my_root].num_pixels;
                        image_stats[rl].sum += image_stats[my_root].sum;
                        image_stats[my_root] = (Stat){};
                        my_root = rl;
                        changed = true;
                    }
                }

                if (row - 1 >= 0) {
                    size_t ru = uf_find_compress(image_roots, idx - x);
                    if (ru != my_root && can_merge(image_size, image_stats, my_root, ru)) {
                        // printf("MERGING %zu and %zu - %zu, %zu\n", ru, my_root, image_stats[ru].sum, image_stats[ru].num_pixels);
                        uf_union(image_roots, ru, my_root);
                        image_stats[ru].num_pixels += image_stats[my_root].num_pixels;
                        image_stats[ru].sum += image_stats[my_root].sum;
                        image_stats[my_root] = (Stat){};
                        my_root = ru;
                        changed = true;
                    }
                }
            }

        }
    }

    print_stats(image_stats, image_size);
    u8 *new_image = mcs_malloc(image_size);
    for (size_t i = 0; i < image_size; ++i) {
        size_t root = uf_find(image_roots, i);
        new_image[i] = image_stats[root].sum / image_stats[root].num_pixels;
    }

    stbi_write_png("image_out.png", x, y, Grey, new_image, x);

    mcs_free(image_roots);
    mcs_free(image_stats);
    stbi_image_free(data);
    fclose(image);
}

size_t uf_find(size_t parents[static 1], size_t element) {
    size_t prev = element;
    for (size_t next = element; next != SIZE_MAX; next = parents[next]) {
        prev = next;
    }
    return prev;
}

size_t uf_find_replace(size_t parents[static 1], size_t element, size_t replace) {
    size_t cur = element;
    for (size_t next = parents[cur]; next != SIZE_MAX; next = parents[cur]) {
        parents[cur] = replace;
        cur = next;
    }
    parents[cur] = replace;
    return replace;
}

size_t uf_find_compress(size_t parents[static 1], size_t element) {
    size_t root = uf_find(parents, element);
    size_t cur = element;
    for (size_t next = parents[cur]; next != SIZE_MAX; next = parents[cur]) {
        parents[cur] = root;
        cur = next;
    }
    return root;
}

void uf_union(size_t parents[static 1], size_t element_1, size_t element_2) {
    size_t root_1 = uf_find_compress(parents, element_1);
    size_t root_2 = uf_find(parents, element_2);
    if (root_1 != root_2) {
        uf_find_replace(parents, element_2, root_1);
    }
}

void calculate_stats(size_t size, u8 image[static 1], size_t parents[static 1], Stat stats[static 1]) {
    bzero(stats, sizeof(Stat) * size);
    for (size_t i = 0; i < size; ++i) {
        size_t root = uf_find_compress(parents, i);
        stats[root].num_pixels += 1;
        stats[root].sum += image[i];
    }
}

void print_stats(Stat stats[static 1], size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (stats[i].num_pixels > 0) {
            printf("ROOT %zu: num_pixels: %zu, sum: %zu\n", i, stats[i].num_pixels, stats[i].sum);
        }
    }
}

bool can_merge(size_t image_size, Stat stats[static 1], size_t root_1, size_t root_2) {
    Stat s1 = stats[root_1];
    Stat s2 = stats[root_2];

    ssize_t avg_val_1 = s1.sum / s1.num_pixels;
    ssize_t avg_val_2 = s2.sum / s2.num_pixels;

    // printf("%zu vs %zu: %zd vs %zd", root_1, root_2, avg_val_1, avg_val_2);

    return labs(avg_val_1 - avg_val_2) <= 20;
}

void usage(char name[static 1]) {
    mcs_eprintfln("Usage: %s <image>", name);
    mcs_eprintfln("Takes an image and group connected regions");
}
