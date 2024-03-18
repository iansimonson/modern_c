#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

double derivative(double f(double), double x0) {
    const double delta = 1.0e-6;
    double x1 = x0 - delta;
    double x2 = x0 + delta;
    double y1 = f(x1);
    double y2 = f(x2);

    return (y2 - y1) / (x2 - x1);
}

int main() {
    double array[100] = {};
    double deriv[100] = {};
    double t = 0.0;
    for (size_t i = 0; i < 100; ++i) {
        array[i] = sin(t);
        deriv[i] = derivative(sin, t);
        t += 0.1;
    }

    printf("SIN: [");
    for (size_t i = 0; i < 25; ++i) {
        printf("%0.3f, ", array[i]);
    }
    printf("]\n");

    printf("deriv(SIN): [");
    for (size_t i = 0; i < 25; ++i) {
        printf("%0.3f, ", deriv[i]);
    }
    printf("]\n");
}
