#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

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
    return malloc(length);
}

void xfree(void *ptr, size_t len) {
    free(ptr);
}

void print_array(double array[static 1], size_t len) {
    printf("[ ");
    for (size_t i = 0; i < len; ++i) {
        printf("%f, ", array[i]);
    }
    printf("]\n");
}

void xquicksort_rec(double array[static 1], size_t lo, size_t hi) {
    if (lo >= hi || lo < 0) {
        return;
    }
    double pivot_val = array[hi];
    size_t pivot_pt = lo - 1;
    for (size_t j = lo; j < hi; ++j) {
        if (array[j] <= pivot_val) {
            pivot_pt += 1;
            swap(array[pivot_pt], array[j]);
        }
    }

    pivot_pt += 1;
    swap(array[hi], array[pivot_pt]);

    xquicksort_rec(array, lo, pivot_pt - 1);
    xquicksort_rec(array, pivot_pt + 1, hi);
}

void xquicksort(double array[static 1], size_t arr_len) {
    xquicksort_rec(array, 0, arr_len - 1);
}

void xmergesort_rec(double array[static 1], double tmp_arr[static 1], size_t start, size_t end) {
    if (end == start || end - start == 1) {
        return;
    } else if (end - start == 2) {
        if (array[start] > array[end - 1]) {
            swap(array[start], array[end - 1]);
        }
        return;
    }

    size_t lhs_end = start + (end - start) / 2;
    size_t rhs_begin = lhs_end;
    size_t array_len = end - start;
    xmergesort_rec(array, tmp_arr, start, lhs_end);
    xmergesort_rec(array, tmp_arr, rhs_begin, end);

    size_t tmp_idx = start;
    size_t lhs = start;
    size_t rhs = rhs_begin;
    for (;lhs < lhs_end && rhs < end;) {
        if (array[lhs] <= array[rhs]) {
            tmp_arr[tmp_idx] = array[lhs];
            tmp_idx += 1;
            lhs += 1;
        } else {
            tmp_arr[tmp_idx] = array[rhs];
            tmp_idx += 1;
            rhs += 1;
        }
    }

    for (;lhs < lhs_end; lhs += 1) {
        tmp_arr[tmp_idx] = array[lhs];
        tmp_idx += 1;
    }
    for (;rhs < end; rhs += 1) {
        tmp_arr[tmp_idx] = array[rhs];
        tmp_idx += 1;
    }
    assert(lhs == lhs_end && rhs == end && tmp_idx == end);
    copy(array + start, tmp_arr + start, array_len);
}

void xmergesort(double array[static 1], size_t arr_len) {
    double *tmp_arr = xmalloc(sizeof(double) * arr_len);
    xmergesort_rec(array, tmp_arr, 0, arr_len);
    xfree(tmp_arr, sizeof(double) * arr_len);
}

int main() {
    printf("Merge Sort\n-----\n");
    double values[5] = {10, 3, 7, 1, 4};
    size_t values_len = array_count(values);
    printf("Before:\n");
    print_array(values, values_len);
    xmergesort(values, 5);
    printf("After:\n");
    print_array(values, values_len);
    printf("\n");

    printf("Quick Sort\n-----\n");
    double values_2[10] = {3, 8, 1, 4, 2, 9, 7, 10, 5, 6};
    size_t values_2_len = array_count(values_2);
    printf("Before:\n");
    print_array(values_2, values_2_len);
    xquicksort(values_2, values_2_len);
    printf("After:\n");
    print_array(values_2, values_2_len);

}
