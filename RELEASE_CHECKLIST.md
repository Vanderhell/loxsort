# LoxSort - Release Checklist

## Locally verified in this workspace

- Debug configure/build/test: passed.
- Release configure/build/test: passed.
- Benchmark target build: passed.
- Install/export package: passed.
- External consumer build/run: passed.
- Clang ASan + UBSan build/test: passed with local LLVM runtime path.
- Full `1,000,000`-dataset release campaign: passed; final report generated from the completed CSV.
- Public documentation refresh: English README, cookbook, API reference, contributing guide, and release notes added.

## Still open

- Fuzz targets are not implemented.
- GitHub Actions workflow has not been executed on GitHub.
- `git diff --check` was not run because this workspace is not a git repository.

## Correctness

- [ ] All algorithms pass exhaustive small-array tests.
- [ ] All algorithms pass property tests against a reference result.
- [ ] Stable algorithms preserve equal-key order.
- [ ] Dispatcher never violates stability or no-recursion constraints.
- [ ] `count * element_size` overflow checks are covered.
- [ ] Debug verification reports unsorted results.

## Portability

- [ ] C99 build passes with `-Wall -Wextra -Wpedantic -Werror`.
- [ ] GCC build passes.
- [ ] Clang build passes.
- [ ] ASan and UBSan pass without findings.
- [ ] The library uses no VLAs, compiler extensions, or linker sections.
- [ ] The library does not call `malloc`, `calloc`, `realloc`, or `free`.

## Runtime policy

- [ ] `count < 2` performs no comparison or write.
- [ ] `count <= small_count_cutoff` skips feature extraction.
- [ ] Feature extraction performs at most 16 comparisons.
- [ ] Sampled indexes stay within `[0, count - 1]`.
- [ ] Invalid or incompatible profiles fall back safely to generic profile.
- [ ] Introspection results report the chosen algorithm and reason.

## Benchmark profiles

- [ ] Data generation is outside the measured interval.
- [ ] Each algorithm gets an identical copy of the same dataset.
- [ ] Dataset seed and timing repetition are separate axes.
- [ ] Profile output includes target identity, toolchain, flags, and algorithms.
- [ ] Profile generation is deterministic.
- [ ] Generated profiles pass validation and regret checks.

## Repository

- [ ] README matches the implemented state.
- [ ] CTest passes in a clean build directory.
- [ ] The working tree is clean.
- [ ] A release tag points at a commit that passed release CI.
