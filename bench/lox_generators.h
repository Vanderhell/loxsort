#ifndef LOXSORT_BENCH_LOX_GENERATORS_H
#define LOXSORT_BENCH_LOX_GENERATORS_H

#include <stddef.h>
#include <stdint.h>

void lox_bench_generate_bytes(
    unsigned char *records,
    size_t count,
    size_t element_size,
    uint32_t seed,
    int pattern);

void lox_bench_copy_bytes(
    unsigned char *dst,
    const unsigned char *src,
    size_t count,
    size_t element_size);

#endif
