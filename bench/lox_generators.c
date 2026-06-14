#include "lox_bench.h"
#include "lox_generators.h"

#include <string.h>

enum {
    LOX_BENCH_MAX_RECORDS = 64u
};

static uint32_t lox_bench_lcg(uint32_t *state)
{
    *state = (*state * 1664525u) + 1013904223u;
    return *state;
}

static uint32_t lox_bench_reverse_bits(uint32_t value, unsigned bits)
{
    uint32_t result = 0u;
    unsigned i;

    for (i = 0u; i < bits; ++i) {
        result <<= 1u;
        result |= value & 1u;
        value >>= 1u;
    }
    return result;
}

static size_t lox_bench_key_size(size_t element_size)
{
    return element_size < sizeof(uint32_t) ? element_size : sizeof(uint32_t);
}

static void lox_bench_write_key(unsigned char *slot, size_t element_size, uint32_t key)
{
    size_t key_size = lox_bench_key_size(element_size);
    size_t j;

    for (j = 0u; j < key_size; ++j) {
        slot[j] = (unsigned char)((key >> (j * 8u)) & 0xffu);
    }
}

static void lox_bench_fill_payload(unsigned char *slot, size_t element_size, uint32_t *state, uint32_t tag)
{
    size_t key_size = lox_bench_key_size(element_size);
    size_t j;

    for (j = key_size; j < element_size; ++j) {
        unsigned char byte = (unsigned char)(lox_bench_lcg(state) & 0xffu);
        slot[j] = (unsigned char)(byte ^ (unsigned char)(tag + (uint32_t)j));
    }
}

static void lox_bench_store_record(
    unsigned char *slot,
    size_t element_size,
    uint32_t key,
    uint32_t seed,
    uint32_t index)
{
    uint32_t payload_state = seed ^ (index * 0x9e3779b9u) ^ (key * 0x85ebca6bu);

    lox_bench_write_key(slot, element_size, key);
    lox_bench_fill_payload(slot, element_size, &payload_state, key ^ index);
}

void lox_bench_generate_bytes(
    unsigned char *records,
    size_t count,
    size_t element_size,
    uint32_t seed,
    int pattern)
{
    size_t i;
    uint32_t keys[LOX_BENCH_MAX_RECORDS];
    uint32_t state = seed;

    if (count > LOX_BENCH_MAX_RECORDS) {
        return;
    }

    for (i = 0u; i < count; ++i) {
        keys[i] = (uint32_t)i;
    }

    switch (pattern) {
    case LOX_BENCH_PATTERN_RANDOM:
        for (i = 0u; i < count; ++i) {
            keys[i] = lox_bench_lcg(&state);
        }
        break;
    case LOX_BENCH_PATTERN_SORTED:
        break;
    case LOX_BENCH_PATTERN_REVERSE:
        for (i = 0u; i < count; ++i) {
            keys[i] = (uint32_t)(count - 1u - i);
        }
        break;
    case LOX_BENCH_PATTERN_NEARLY_SORTED:
        for (i = 1u; i + 1u < count; i += 4u) {
            uint32_t tmp = keys[i];
            keys[i] = keys[i + 1u];
            keys[i + 1u] = tmp;
        }
        break;
    case LOX_BENCH_PATTERN_FEW_UNIQUE_2:
    case LOX_BENCH_PATTERN_FEW_UNIQUE_8: {
        uint32_t modulo = pattern == LOX_BENCH_PATTERN_FEW_UNIQUE_2 ? 2u : 8u;
        for (i = 0u; i < count; ++i) {
            keys[i] = lox_bench_lcg(&state) % modulo;
        }
        break;
    }
    case LOX_BENCH_PATTERN_ALL_EQUAL:
        for (i = 0u; i < count; ++i) {
            keys[i] = 1u;
        }
        break;
    case LOX_BENCH_PATTERN_ORGAN_PIPE:
        for (i = 0u; i < count; ++i) {
            keys[i] = (uint32_t)(i <= count / 2u ? i : (count - i));
    }
    break;
    case LOX_BENCH_PATTERN_STAGGER: {
        size_t stride = count / 2u + 1u;
        if (stride == 0u) {
            stride = 1u;
        }
        for (i = 0u; i < count; ++i) {
            keys[i] = (uint32_t)(((i * stride) + i) % count);
        }
        break;
    }
    case LOX_BENCH_PATTERN_PLATEAU: {
        uint32_t plateau = (uint32_t)(count / 4u);
        for (i = 0u; i < count; ++i) {
            keys[i] = (uint32_t)(i < plateau ? i : plateau);
        }
        break;
    }
    case LOX_BENCH_PATTERN_SAWTOOTH:
        for (i = 0u; i < count; ++i) {
            keys[i] = (uint32_t)(i % 7u);
        }
        break;
    case LOX_BENCH_PATTERN_SHUFFLED_RUNS:
        for (i = 0u; i < count; ++i) {
            keys[i] = (uint32_t)((i / 4u) * 4u + (3u - (i % 4u)));
        }
        break;
    case LOX_BENCH_PATTERN_REVERSE_FIRST_HALF:
        for (i = 0u; i < count / 2u; ++i) {
            keys[i] = (uint32_t)((count / 2u) - 1u - i);
        }
        for (i = count / 2u; i < count; ++i) {
            keys[i] = (uint32_t)i;
        }
        break;
    case LOX_BENCH_PATTERN_REVERSE_SECOND_HALF:
        for (i = 0u; i < count / 2u; ++i) {
            keys[i] = (uint32_t)i;
        }
        for (i = count / 2u; i < count; ++i) {
            keys[i] = (uint32_t)(count - 1u - (i - count / 2u));
        }
        break;
    case LOX_BENCH_PATTERN_RANDOM_HALF:
        for (i = 0u; i < count / 2u; ++i) {
            keys[i] = lox_bench_lcg(&state);
        }
        for (i = count / 2u; i < count; ++i) {
            keys[i] = (uint32_t)i;
        }
        break;
    case LOX_BENCH_PATTERN_RANDOM_TAIL: {
        size_t tail = count / 4u;
        for (i = 0u; i < count - tail; ++i) {
            keys[i] = (uint32_t)i;
        }
        for (; i < count; ++i) {
            keys[i] = lox_bench_lcg(&state);
        }
        break;
    }
    case LOX_BENCH_PATTERN_PUSH_FRONT:
        if (count > 0u) {
            keys[0] = (uint32_t)(count - 1u);
            for (i = 1u; i < count; ++i) {
                keys[i] = (uint32_t)(i - 1u);
            }
        }
        break;
    case LOX_BENCH_PATTERN_PUSH_MIDDLE:
        for (i = 0u; i < count / 2u; ++i) {
            keys[i] = (uint32_t)i;
        }
        if (count > 0u) {
            keys[count / 2u] = (uint32_t)(count - 1u);
        }
        for (i = count / 2u + 1u; i < count; ++i) {
            keys[i] = (uint32_t)(i - 1u);
        }
        break;
    case LOX_BENCH_PATTERN_ASCENDING_RUNS: {
        size_t run_length = 4u;
        size_t run_count = (count + run_length - 1u) / run_length;
        for (i = 0u; i < count; ++i) {
            size_t run = i / run_length;
            size_t offset = i % run_length;
            keys[i] = (uint32_t)((run_count - 1u - run) * run_length + offset);
        }
        break;
    }
    case LOX_BENCH_PATTERN_DESCENDING_RUNS: {
        size_t run_length = 4u;
        for (i = 0u; i < count; ++i) {
            size_t run = i / run_length;
            size_t offset = i % run_length;
            keys[i] = (uint32_t)(run * run_length + (run_length - 1u - offset));
        }
        break;
    }
    case LOX_BENCH_PATTERN_ALTERNATING_HIGH_LOW:
        for (i = 0u; i < count; ++i) {
            keys[i] = (uint32_t)((i % 2u == 0u) ? (i / 2u) : (count - 1u - (i / 2u)));
        }
        break;
    case LOX_BENCH_PATTERN_BIT_REVERSAL: {
        unsigned bits = 0u;
        uint32_t domain = 1u;
        while (domain < count) {
            domain <<= 1u;
            ++bits;
        }
        if (bits == 0u) {
            bits = 1u;
        }
        for (i = 0u; i < count; ++i) {
            uint32_t reversed = lox_bench_reverse_bits((uint32_t)i, bits);
            keys[i] = count == 0u ? 0u : (reversed % (uint32_t)count);
        }
        break;
    }
    case LOX_BENCH_PATTERN_ADVERSARIAL:
        for (i = 0u; i < count; ++i) {
            keys[i] = (uint32_t)((i * 37u) ^ (i >> 1u));
        }
        break;
    default:
        for (i = 0u; i < count; ++i) {
            keys[i] = lox_bench_lcg(&state);
        }
        break;
    }

    for (i = 0u; i < count; ++i) {
        lox_bench_store_record(
            records + (i * element_size),
            element_size,
            keys[i],
            seed,
            (uint32_t)i);
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
