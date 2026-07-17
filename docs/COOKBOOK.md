# LoxSort Cookbook

This page gives a concise, practical starting point for using and integrating LoxSort.

## Build and test

```text
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

## Recommended CMake options

- `LOXSORT_BUILD_TESTS`
- `LOXSORT_BUILD_BENCH`
- `LOXSORT_ENABLE_CYCLE`
- `LOXSORT_ENABLE_VERIFY`
- `LOXSORT_ENABLE_SANITIZERS`

## Integration model

LoxSort is designed to be used as a stateless C99 library:

- supply the array pointer, count, element size, comparator, and optional sort options;
- use `lox_profile_generic` for a safe default profile;
- inspect `lox_sort_result_t` when you need the chosen algorithm or decision reason;
- keep custom scratch buffers and profile data owned by the caller.

## Minimal example

```c
#include "loxsort/loxsort.h"

static int int_compare(const void *lhs, const void *rhs, void *user) {
    (void)user;
    const int left = *(const int *)lhs;
    const int right = *(const int *)rhs;
    return (left > right) - (left < right);
}

void sort_items(int *items, size_t count) {
    lox_sort_options_t options = {0};
    lox_sort_result_t result = {0};

    options.profile = &lox_profile_generic;

    (void)lox_sort(
        items,
        count,
        sizeof(*items),
        int_compare,
        NULL,
        &options,
        &result);
}
```

## Practical guidance

- Use `LOX_SORT_VERIFY_RESULT` during validation builds.
- Keep benchmark-generated artifacts out of source commits.
- Prefer the dispatcher over hard-coding algorithm selection in application code.
- Treat the benchmark/profile pipeline as a tuning aid, not as runtime self-learning.
