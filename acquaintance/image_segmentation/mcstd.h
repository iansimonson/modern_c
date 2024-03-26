#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
#define MCS_ASSERT(cond, message) assert(((message), (cond)))
#pragma GCC disagnostic pop
#pragma clang diagnostic pop


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;


static inline void *mcs_malloc(size_t size) {
    void *ptr = malloc(size);
    MCS_ASSERT(ptr != NULL, "malloc returned null pointer");
    return ptr;
}

static inline void *mcs_calloc(size_t size) {
    void *ptr = calloc(1, size);
    MCS_ASSERT(ptr != NULL, "malloc returned null pointer");
    return ptr;
}

static inline void *mcs_make_array(size_t num_elements, size_t element_size) {
    void *ptr = calloc(num_elements, element_size);
    MCS_ASSERT(ptr != NULL, "malloc returned null pointer");
    return ptr;
}

static inline void mcs_free(void *ptr) {
    free(ptr);
}

static inline int mcs_eprintf(char const *format, ...) {
    int ret;
    va_list args;
    va_start(args, format);
    ret = vfprintf(stderr, format, args);
    va_end(args);
    return ret;
}

static inline int mcs_eprintfln(char const *format, ...) {
    int ret;
    va_list args;
    va_start(args, format);
    ret = vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
    return ret;
}