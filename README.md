# LoxSort

[![CI](https://github.com/Vanderhell/loxsort/actions/workflows/ci.yml/badge.svg)](https://github.com/Vanderhell/loxsort/actions/workflows/ci.yml)
[![Issues](https://img.shields.io/badge/issues-tracker-blue)](https://github.com/Vanderhell/loxsort/issues)

LoxSort is an experimental deterministic C99 sorting library for embedded and resource-constrained systems. It selects an algorithm based on input size, stability requirements, scratch-buffer availability, recursion constraints, and an offline benchmark profile.

## Highlights

- Deterministic dispatcher with explicit policy rules.
- Insertion, Shell, introsort, and bottom-up merge sort.
- Optional cycle sort for specialized builds.
- Stateless public API.
- Offline benchmark and profile generation pipeline.
- CMake export and install support.
- Test suite covering API, algorithms, dispatch, profile logic, and stability.

## Release Status

- Full `1,000,000`-dataset release benchmark: completed in this workspace.

The current release evidence shows that the dispatcher is correct and measurable at scale, but it is not globally optimal against the oracle and fixed-strategy baselines on every aggregated view.

## Assessment

LoxSort is a valid experimental project if the goal is a deterministic, constraint-aware sorting library for embedded or otherwise resource-sensitive environments.

Its value is in predictable behavior, explicit policy control, and a benchmark-backed dispatcher that respects stability, scratch-buffer availability, and recursion constraints.

The benchmark results do not support positioning it as a universal performance winner. They do support it as a practical, auditable experimental runtime with clear tradeoffs and a documented decision model.

## Cookbook

This section is the practical starting point for users and integrators.

### Build

```text
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

### Useful CMake options

- `LOXSORT_BUILD_TESTS`
- `LOXSORT_BUILD_BENCH`
- `LOXSORT_ENABLE_CYCLE`
- `LOXSORT_ENABLE_VERIFY`
- `LOXSORT_ENABLE_SANITIZERS`

### Suggested reading order

1. [`docs/LOXSORT_SPEC.md`](docs/LOXSORT_SPEC.md)
2. [`docs/API_CONTRACT.md`](docs/API_CONTRACT.md)
3. [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md)
4. [`docs/ALGORITHM_POLICY.md`](docs/ALGORITHM_POLICY.md)
5. [`docs/BENCHMARK_AND_PROFILES.md`](docs/BENCHMARK_AND_PROFILES.md)

## Example

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

## Documentation

- [`docs/COOKBOOK.md`](docs/COOKBOOK.md)
- [`docs/API_REFERENCE.md`](docs/API_REFERENCE.md)
- [`docs/CONTRIBUTING.md`](docs/CONTRIBUTING.md)
- [`docs/RELEASE_NOTES.md`](docs/RELEASE_NOTES.md)
- [`docs/LOXSORT_SPEC.md`](docs/LOXSORT_SPEC.md)
- [`docs/API_CONTRACT.md`](docs/API_CONTRACT.md)
- [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md)
- [`docs/ALGORITHM_POLICY.md`](docs/ALGORITHM_POLICY.md)
- [`docs/BENCHMARK_AND_PROFILES.md`](docs/BENCHMARK_AND_PROFILES.md)
- [`docs/TEST_STRATEGY.md`](docs/TEST_STRATEGY.md)
- [`docs/FILE_LAYOUT.md`](docs/FILE_LAYOUT.md)
- [`docs/DECISIONS.md`](docs/DECISIONS.md)

## Issues

Use the GitHub issue tracker for bugs, regressions, feature requests, and benchmark anomalies:

https://github.com/Vanderhell/loxsort/issues

When reporting a problem, include:

- target platform and compiler;
- build configuration and enabled CMake options;
- minimal reproduction steps;
- expected vs actual behavior;
- any benchmark CSV or report excerpt that supports the claim.

## Contributing

Contributions are welcome if they preserve the library's deterministic, constraint-aware design.

Before opening a pull request:

- build the project in Debug and Release;
- run the test suite;
- verify any benchmark or profile changes against the existing docs;
- keep generated artifacts out of source changes unless they are intentionally updated;
- update the relevant documentation when behavior changes.

## License

See [`LICENSE`](LICENSE) for the current distribution terms.
