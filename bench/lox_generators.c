#include "lox_bench.h"
#include "lox_generators.h"

#include <string.h>

static uint32_t lox_bench_lcg(uint32_t *state)
{
    *state = (*state * 1664525u) + 1013904223u;
    return *state;
}

void lox_bench_generate_bytes(
    unsigned char *records,
    size_t count,
    size_t element_size,
    uint32_t seed,
    int pattern)
{
    size_t i;
    uint32_t state = seed;

    for (i = 0u; i < count; ++i) {
        unsigned char *slot = records + (i * element_size);
        size_t j;
        uint32_t key = (uint32_t)i;
        if (element_size < sizeof(uint32_t)) {
            key = lox_bench_lcg(&state);
        }
        for (j = 0u; j < element_size; ++j) {
            slot[j] = (unsigned char)(lox_bench_lcg(&state) & 0xffu);
        }
        if (element_size >= sizeof(uint32_t)) {
            memcpy(slot, &key, sizeof(key));
        } else {
            for (j = 0u; j < element_size; ++j) {
                slot[j] = (unsigned char)((key >> (j * 8u)) & 0xffu);
            }
        }
    }

    if (pattern == LOX_BENCH_PATTERN_SORTED) {
        return;
    }

    if (pattern == LOX_BENCH_PATTERN_REVERSE) {
        for (i = 0u; i < count / 2u; ++i) {
            unsigned char tmp[32];
            memcpy(tmp, records + (i * element_size), element_size);
            memcpy(records + (i * element_size), records + ((count - 1u - i) * element_size), element_size);
            memcpy(records + ((count - 1u - i) * element_size), tmp, element_size);
        }
        return;
    }

    if (pattern == LOX_BENCH_PATTERN_NEARLY_SORTED) {
        for (i = 1u; i < count; i += 4u) {
            if (i + 1u < count) {
                unsigned char tmp[32];
                memcpy(tmp, records + (i * element_size), element_size);
                memcpy(records + (i * element_size), records + ((i + 1u) * element_size), element_size);
                memcpy(records + ((i + 1u) * element_size), tmp, element_size);
            }
        }
        return;
    }

    if (pattern == LOX_BENCH_PATTERN_FEW_UNIQUE_2 || pattern == LOX_BENCH_PATTERN_FEW_UNIQUE_8) {
        uint32_t modulo = pattern == LOX_BENCH_PATTERN_FEW_UNIQUE_2 ? 2u : 8u;
        for (i = 0u; i < count; ++i) {
            uint32_t key = lox_bench_lcg(&state) % modulo;
            unsigned char *slot = records + (i * element_size);
            size_t j;
            for (j = 0u; j < element_size; ++j) {
                slot[j] = (unsigned char)((key >> ((j % sizeof(uint32_t)) * 8u)) & 0xffu);
            }
        }
        return;
    }

    if (pattern == LOX_BENCH_PATTERN_ALL_EQUAL) {
        for (i = 0u; i < count; ++i) {
            memset(records + (i * element_size), 1, element_size);
        }
        return;
    }

    if (pattern == LOX_BENCH_PATTERN_ORGAN_PIPE) {
        for (i = 0u; i < count; ++i) {
            uint32_t key = (uint32_t)(i <= count / 2u ? i : (count - i));
            memcpy(records + (i * element_size), &key, element_size < sizeof(key) ? element_size : sizeof(key));
        }
        return;
    }

    if (pattern == LOX_BENCH_PATTERN_SAWTOOTH) {
        for (i = 0u; i < count; ++i) {
            uint32_t key = (uint32_t)(i % 7u);
            memcpy(records + (i * element_size), &key, element_size < sizeof(key) ? element_size : sizeof(key));
        }
        return;
    }

    if (pattern == LOX_BENCH_PATTERN_SHUFFLED_RUNS) {
        for (i = 0u; i < count; ++i) {
            uint32_t key = (uint32_t)((i / 4u) * 4u + (3u - (i % 4u)));
            memcpy(records + (i * element_size), &key, element_size < sizeof(key) ? element_size : sizeof(key));
        }
        return;
    }

    if (pattern == LOX_BENCH_PATTERN_ADVERSARIAL) {
        for (i = 0u; i < count; ++i) {
            uint32_t key = (uint32_t)((i * 37u) ^ (i >> 1u));
            memcpy(records + (i * element_size), &key, element_size < sizeof(key) ? element_size : sizeof(key));
        }
        return;
    }
}

void lox_bench_copy_bytes(
    unsigned char *dst,
    const unsigned char *src,
    size_t count,
    size_t element_size)
{
    size_t i;
    for (i = 0u; i < count; ++i) {
        memcpy(dst + (i * element_size), src + (i * element_size), element_size);
    }
}
