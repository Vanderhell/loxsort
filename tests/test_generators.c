#include "test_support.h"

#include "../bench/lox_bench.h"
#include "../bench/lox_generators.h"

enum {
    TEST_MAX_RECORDS = 64u,
    TEST_MAX_ELEMENT_SIZE = 32u
};

static int test_compare_u32_qsort(const void *lhs, const void *rhs)
{
    uint32_t a = *(const uint32_t *)lhs;
    uint32_t b = *(const uint32_t *)rhs;
    if (a < b) {
        return -1;
    }
    if (a > b) {
        return 1;
    }
    return 0;
}

static uint32_t test_read_key(const unsigned char *slot, size_t element_size)
{
    uint32_t key = 0u;
    size_t key_size = element_size < sizeof(uint32_t) ? element_size : sizeof(uint32_t);
    size_t i;

    for (i = 0u; i < key_size; ++i) {
        key |= ((uint32_t)slot[i]) << (i * 8u);
    }
    return key;
}

static void test_extract_keys(uint32_t *keys, const unsigned char *records, size_t count, size_t element_size)
{
    size_t i;
    for (i = 0u; i < count; ++i) {
        keys[i] = test_read_key(records + (i * element_size), element_size);
    }
}

static size_t test_unique_count(const uint32_t *keys, size_t count)
{
    uint32_t copy[TEST_MAX_RECORDS];
    size_t i;
    size_t unique_count;

    REQUIRE(count <= TEST_MAX_RECORDS);
    for (i = 0u; i < count; ++i) {
        copy[i] = keys[i];
    }
    qsort(copy, count, sizeof(copy[0]), test_compare_u32_qsort);
    unique_count = count == 0u ? 0u : 1u;
    for (i = 1u; i < count; ++i) {
        if (copy[i] != copy[i - 1u]) {
            ++unique_count;
        }
    }
    return unique_count;
}

static void test_generate(
    unsigned char *buffer,
    size_t count,
    size_t element_size,
    uint32_t seed,
    lox_bench_pattern_t pattern)
{
    memset(buffer, 0xA5, count * element_size);
    lox_bench_generate_bytes(buffer, count, element_size, seed, pattern);
}

static void test_expect_deterministic(
    size_t count,
    size_t element_size,
    uint32_t seed,
    lox_bench_pattern_t pattern)
{
    unsigned char a[TEST_MAX_RECORDS * TEST_MAX_ELEMENT_SIZE];
    unsigned char b[TEST_MAX_RECORDS * TEST_MAX_ELEMENT_SIZE];
    size_t size = count * element_size;

    REQUIRE(size <= sizeof(a));
    test_generate(a, count, element_size, seed, pattern);
    test_generate(b, count, element_size, seed, pattern);
    REQUIRE(memcmp(a, b, size) == 0);
}

static void test_expect_key_sequence(
    size_t count,
    size_t element_size,
    uint32_t seed,
    lox_bench_pattern_t pattern,
    const uint32_t *expected)
{
    unsigned char buffer[TEST_MAX_RECORDS * TEST_MAX_ELEMENT_SIZE];
    uint32_t keys[TEST_MAX_RECORDS];
    size_t i;
    size_t size = count * element_size;

    REQUIRE(size <= sizeof(buffer));
    test_generate(buffer, count, element_size, seed, pattern);
    test_extract_keys(keys, buffer, count, element_size);
    for (i = 0u; i < count; ++i) {
        REQUIRE(keys[i] == expected[i]);
    }
}

static void test_expect_raw_sorted(
    size_t count,
    size_t element_size,
    uint32_t seed,
    lox_bench_pattern_t pattern,
    int ascending)
{
    unsigned char buffer[TEST_MAX_RECORDS * TEST_MAX_ELEMENT_SIZE];
    size_t key_size = element_size < sizeof(uint32_t) ? element_size : sizeof(uint32_t);
    size_t i;

    REQUIRE(count * element_size <= sizeof(buffer));
    test_generate(buffer, count, element_size, seed, pattern);
    for (i = 1u; i < count; ++i) {
        int cmp = test_compare_raw_key(
            buffer + ((i - 1u) * element_size),
            buffer + (i * element_size),
            &key_size);
        if (ascending) {
            REQUIRE(cmp <= 0);
        } else {
            REQUIRE(cmp >= 0);
        }
    }
}

static uint32_t test_lcg_next(uint32_t *state)
{
    *state = (*state * 1664525u) + 1013904223u;
    return *state;
}

static uint32_t test_reverse_bits(uint32_t value, unsigned bits)
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

static void test_fill_expected_random_half(uint32_t *expected, size_t count, uint32_t seed)
{
    size_t i;
    uint32_t state = seed;

    for (i = 0u; i < count / 2u; ++i) {
        expected[i] = test_lcg_next(&state);
    }
    for (i = count / 2u; i < count; ++i) {
        expected[i] = (uint32_t)i;
    }
}

static void test_fill_expected_random_tail(uint32_t *expected, size_t count, uint32_t seed)
{
    size_t i;
    size_t tail = count / 4u;
    uint32_t state = seed;

    for (i = 0u; i < count - tail; ++i) {
        expected[i] = (uint32_t)i;
    }
    for (; i < count; ++i) {
        expected[i] = test_lcg_next(&state);
    }
}

static void test_fill_expected_stagger(uint32_t *expected, size_t count)
{
    size_t i;
    size_t stride = count / 2u + 1u;

    for (i = 0u; i < count; ++i) {
        expected[i] = (uint32_t)(((i * stride) + i) % count);
    }
}

static void test_fill_expected_plateau(uint32_t *expected, size_t count)
{
    size_t i;
    uint32_t plateau = (uint32_t)(count / 4u);

    for (i = 0u; i < count; ++i) {
        expected[i] = (uint32_t)(i < plateau ? i : plateau);
    }
}

static void test_fill_expected_push_front(uint32_t *expected, size_t count)
{
    size_t i;

    if (count == 0u) {
        return;
    }
    expected[0] = (uint32_t)(count - 1u);
    for (i = 1u; i < count; ++i) {
        expected[i] = (uint32_t)(i - 1u);
    }
}

static void test_fill_expected_push_middle(uint32_t *expected, size_t count)
{
    size_t i;

    for (i = 0u; i < count / 2u; ++i) {
        expected[i] = (uint32_t)i;
    }
    if (count > 0u) {
        expected[count / 2u] = (uint32_t)(count - 1u);
    }
    for (i = count / 2u + 1u; i < count; ++i) {
        expected[i] = (uint32_t)(i - 1u);
    }
}

static void test_fill_expected_ascending_runs(uint32_t *expected, size_t count)
{
    size_t i;
    size_t run_length = 4u;
    size_t run_count = (count + run_length - 1u) / run_length;

    for (i = 0u; i < count; ++i) {
        size_t run = i / run_length;
        size_t offset = i % run_length;
        expected[i] = (uint32_t)((run_count - 1u - run) * run_length + offset);
    }
}

static void test_fill_expected_descending_runs(uint32_t *expected, size_t count)
{
    size_t i;
    size_t run_length = 4u;

    for (i = 0u; i < count; ++i) {
        size_t run = i / run_length;
        size_t offset = i % run_length;
        expected[i] = (uint32_t)(run * run_length + (run_length - 1u - offset));
    }
}

static void test_fill_expected_alternating_high_low(uint32_t *expected, size_t count)
{
    size_t i;
    for (i = 0u; i < count; ++i) {
        expected[i] = (uint32_t)((i % 2u == 0u) ? (i / 2u) : (count - 1u - (i / 2u)));
    }
}

static void test_fill_expected_bit_reversal(uint32_t *expected, size_t count)
{
    size_t i;
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
        uint32_t reversed = test_reverse_bits((uint32_t)i, bits);
        expected[i] = count == 0u ? 0u : (reversed % (uint32_t)count);
    }
}

void test_generators_suite(void)
{
    uint32_t keys[TEST_MAX_RECORDS];
    uint32_t expected[TEST_MAX_RECORDS];
    unsigned char buffer[TEST_MAX_RECORDS * TEST_MAX_ELEMENT_SIZE];
    size_t i;
    static const lox_bench_pattern_t deterministic_patterns[] = {
        LOX_BENCH_PATTERN_RANDOM,
        LOX_BENCH_PATTERN_SORTED,
        LOX_BENCH_PATTERN_REVERSE,
        LOX_BENCH_PATTERN_NEARLY_SORTED,
        LOX_BENCH_PATTERN_FEW_UNIQUE_2,
        LOX_BENCH_PATTERN_FEW_UNIQUE_8,
        LOX_BENCH_PATTERN_ALL_EQUAL,
        LOX_BENCH_PATTERN_ORGAN_PIPE,
        LOX_BENCH_PATTERN_STAGGER,
        LOX_BENCH_PATTERN_PLATEAU,
        LOX_BENCH_PATTERN_SAWTOOTH,
        LOX_BENCH_PATTERN_DESCENDING_BLOCKS_4,
        LOX_BENCH_PATTERN_REVERSE_FIRST_HALF,
        LOX_BENCH_PATTERN_REVERSE_SECOND_HALF,
        LOX_BENCH_PATTERN_RANDOM_HALF,
        LOX_BENCH_PATTERN_RANDOM_TAIL,
        LOX_BENCH_PATTERN_PUSH_FRONT,
        LOX_BENCH_PATTERN_PUSH_MIDDLE,
        LOX_BENCH_PATTERN_ASCENDING_RUNS,
        LOX_BENCH_PATTERN_DESCENDING_RUNS,
        LOX_BENCH_PATTERN_ALTERNATING_HIGH_LOW,
        LOX_BENCH_PATTERN_BIT_REVERSAL,
        LOX_BENCH_PATTERN_ADVERSARIAL
    };

    TEST_CASE("deterministic-across-patterns");
    for (i = 0u; i < sizeof(deterministic_patterns) / sizeof(deterministic_patterns[0]); ++i) {
        test_expect_deterministic(16u, 8u, 1u + (uint32_t)(i * 17u), deterministic_patterns[i]);
    }
    test_expect_deterministic(32u, 32u, 123u, LOX_BENCH_PATTERN_DESCENDING_BLOCKS_4);
    test_expect_deterministic(32u, 16u, 123u, LOX_BENCH_PATTERN_ADVERSARIAL);

    TEST_CASE("sorted-and-reverse-shape");
    for (i = 0u; i < 16u; ++i) {
        expected[i] = (uint32_t)i;
    }
    test_expect_key_sequence(16u, 4u, 1u, LOX_BENCH_PATTERN_SORTED, expected);
    for (i = 0u; i < 16u; ++i) {
        expected[i] = (uint32_t)(15u - i);
    }
    test_expect_key_sequence(16u, 4u, 1u, LOX_BENCH_PATTERN_REVERSE, expected);

    TEST_CASE("nearly-sorted-exact-swaps");
    for (i = 0u; i < 16u; ++i) {
        expected[i] = (uint32_t)i;
    }
    for (i = 1u; i + 1u < 16u; i += 4u) {
        uint32_t tmp = expected[i];
        expected[i] = expected[i + 1u];
        expected[i + 1u] = tmp;
    }
    test_expect_key_sequence(16u, 4u, 1u, LOX_BENCH_PATTERN_NEARLY_SORTED, expected);

    TEST_CASE("few-unique");
    test_generate(buffer, 64u, 4u, 1u, LOX_BENCH_PATTERN_FEW_UNIQUE_2);
    test_extract_keys(keys, buffer, 64u, 4u);
    REQUIRE(test_unique_count(keys, 64u) == 2u);
    for (i = 0u; i < 64u; ++i) {
        REQUIRE(keys[i] <= 1u);
    }

    test_generate(buffer, 64u, 4u, 1u, LOX_BENCH_PATTERN_FEW_UNIQUE_8);
    test_extract_keys(keys, buffer, 64u, 4u);
    REQUIRE(test_unique_count(keys, 64u) == 8u);
    for (i = 0u; i < 64u; ++i) {
        REQUIRE(keys[i] < 8u);
    }

    TEST_CASE("all-equal");
    test_generate(buffer, 32u, 32u, 1u, LOX_BENCH_PATTERN_ALL_EQUAL);
    test_extract_keys(keys, buffer, 32u, 32u);
    REQUIRE(test_unique_count(keys, 32u) == 1u);
    for (i = 0u; i < 32u; ++i) {
        REQUIRE(keys[i] == 1u);
    }

    TEST_CASE("organ-pipe");
    test_generate(buffer, 17u, 4u, 1u, LOX_BENCH_PATTERN_ORGAN_PIPE);
    test_extract_keys(keys, buffer, 17u, 4u);
    for (i = 1u; i <= 17u / 2u; ++i) {
        REQUIRE(keys[i - 1u] <= keys[i]);
    }
    for (i = 17u / 2u + 1u; i < 17u; ++i) {
        REQUIRE(keys[i - 1u] >= keys[i]);
    }
    REQUIRE(keys[17u / 2u] >= keys[17u / 2u - 1u]);

    TEST_CASE("stagger");
    test_fill_expected_stagger(expected, 16u);
    test_expect_key_sequence(16u, 4u, 1u, LOX_BENCH_PATTERN_STAGGER, expected);

    TEST_CASE("plateau");
    test_fill_expected_plateau(expected, 16u);
    test_expect_key_sequence(16u, 4u, 1u, LOX_BENCH_PATTERN_PLATEAU, expected);

    TEST_CASE("sawtooth");
    test_generate(buffer, 28u, 4u, 1u, LOX_BENCH_PATTERN_SAWTOOTH);
    test_extract_keys(keys, buffer, 28u, 4u);
    for (i = 0u; i < 28u; ++i) {
        REQUIRE(keys[i] == (uint32_t)(i % 7u));
    }

    TEST_CASE("descending-blocks-4");
    test_generate(buffer, 16u, 4u, 1u, LOX_BENCH_PATTERN_DESCENDING_BLOCKS_4);
    test_extract_keys(keys, buffer, 16u, 4u);
    for (i = 0u; i < 16u; ++i) {
        REQUIRE(keys[i] == (uint32_t)((i / 4u) * 4u + (3u - (i % 4u))));
    }
    REQUIRE(test_unique_count(keys, 16u) == 16u);

    TEST_CASE("reverse-halves");
    for (i = 0u; i < 8u; ++i) {
        expected[i] = (uint32_t)(7u - i);
    }
    for (i = 8u; i < 16u; ++i) {
        expected[i] = (uint32_t)i;
    }
    test_expect_key_sequence(16u, 4u, 1u, LOX_BENCH_PATTERN_REVERSE_FIRST_HALF, expected);
    for (i = 0u; i < 8u; ++i) {
        expected[i] = (uint32_t)i;
    }
    for (i = 8u; i < 16u; ++i) {
        expected[i] = (uint32_t)(15u - (i - 8u));
    }
    test_expect_key_sequence(16u, 4u, 1u, LOX_BENCH_PATTERN_REVERSE_SECOND_HALF, expected);

    TEST_CASE("random-half-and-tail");
    test_fill_expected_random_half(expected, 16u, 1u);
    test_expect_key_sequence(16u, 4u, 1u, LOX_BENCH_PATTERN_RANDOM_HALF, expected);
    test_fill_expected_random_tail(expected, 16u, 1u);
    test_expect_key_sequence(16u, 4u, 1u, LOX_BENCH_PATTERN_RANDOM_TAIL, expected);

    TEST_CASE("push-front-middle");
    test_fill_expected_push_front(expected, 16u);
    test_expect_key_sequence(16u, 4u, 1u, LOX_BENCH_PATTERN_PUSH_FRONT, expected);
    test_fill_expected_push_middle(expected, 16u);
    test_expect_key_sequence(16u, 4u, 1u, LOX_BENCH_PATTERN_PUSH_MIDDLE, expected);

    TEST_CASE("run-shapes");
    test_fill_expected_ascending_runs(expected, 16u);
    test_expect_key_sequence(16u, 4u, 1u, LOX_BENCH_PATTERN_ASCENDING_RUNS, expected);
    test_fill_expected_descending_runs(expected, 16u);
    test_expect_key_sequence(16u, 4u, 1u, LOX_BENCH_PATTERN_DESCENDING_RUNS, expected);

    TEST_CASE("alternating-high-low");
    test_fill_expected_alternating_high_low(expected, 16u);
    test_expect_key_sequence(16u, 4u, 1u, LOX_BENCH_PATTERN_ALTERNATING_HIGH_LOW, expected);

    TEST_CASE("bit-reversal");
    test_fill_expected_bit_reversal(expected, 16u);
    test_expect_key_sequence(16u, 4u, 1u, LOX_BENCH_PATTERN_BIT_REVERSAL, expected);

    TEST_CASE("adversarial");
    test_generate(buffer, 32u, 8u, 1u, LOX_BENCH_PATTERN_ADVERSARIAL);
    test_extract_keys(keys, buffer, 32u, 8u);
    REQUIRE(test_unique_count(keys, 32u) == 32u);
    REQUIRE(test_is_sorted_raw(buffer, 32u, 8u, sizeof(uint32_t)) == 0);

    TEST_CASE("small-element-sizes");
    test_expect_deterministic(16u, 1u, 7u, LOX_BENCH_PATTERN_SORTED);
    test_expect_deterministic(16u, 2u, 7u, LOX_BENCH_PATTERN_REVERSE);
    test_expect_deterministic(16u, 16u, 7u, LOX_BENCH_PATTERN_NEARLY_SORTED);
    test_expect_raw_sorted(16u, 1u, 7u, LOX_BENCH_PATTERN_SORTED, 1);
    test_expect_raw_sorted(16u, 2u, 7u, LOX_BENCH_PATTERN_REVERSE, 0);

    TEST_CASE("seed-variation");
    test_generate(buffer, 16u, 8u, 1u, LOX_BENCH_PATTERN_RANDOM);
    test_extract_keys(keys, buffer, 16u, 8u);
    {
        unsigned char other[16u * 8u];
        test_generate(other, 16u, 8u, 7u, LOX_BENCH_PATTERN_RANDOM);
        REQUIRE(memcmp(buffer, other, 16u * 8u) != 0);
    }
}
