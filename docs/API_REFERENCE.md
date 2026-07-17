# LoxSort API Reference

This document is a compact public reference for the exported API.
The normative contract remains [`API_CONTRACT.md`](API_CONTRACT.md).

## Public header

```text
include/loxsort/loxsort.h
```

## Core types

- `lox_compare_fn`
- `lox_status_t`
- `lox_algorithm_t`
- `lox_decision_reason_t`
- `lox_profile_t`
- `lox_sort_options_t`
- `lox_sort_result_t`

## Core entry point

```c
lox_status_t lox_sort(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options,
    lox_sort_result_t *result);
```

## Supporting helpers

- `lox_merge_scratch_required()`
- `lox_algorithm_name()`
- `lox_status_name()`
- `lox_profile_generic`

## Contract summary

- `count < 2` is a trivial no-op and returns success.
- `options == NULL` is valid and falls back to the generic profile.
- `result == NULL` is valid if the caller does not need diagnostics.
- `compare` must provide a total order consistent with the stored data.
- The caller owns the input buffer, scratch buffer, and any comparator state.

## Result reporting

`lox_sort_result_t` reports the selected algorithm and the decision reason so callers can understand why a particular path was chosen.

For deeper behavioral rules, see:

- [`API_CONTRACT.md`](API_CONTRACT.md)
- [`LOXSORT_SPEC.md`](LOXSORT_SPEC.md)
- [`ALGORITHM_POLICY.md`](ALGORITHM_POLICY.md)
