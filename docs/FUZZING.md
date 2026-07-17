# LoxSort Fuzzing

LoxSort includes public fuzz targets to validate the dispatcher and the public API under sanitizer coverage.

## Targets

- `loxsort_fuzz_public_api`
- `loxsort_fuzz_algorithms`

## Build

Fuzz targets require Clang or `clang-cl` and the sanitizers enabled through CMake:

```text
cmake -S . -B build -DLOXSORT_BUILD_FUZZ=ON -DLOXSORT_ENABLE_SANITIZERS=ON
cmake --build build --config Debug
```

## Run

Example invocation:

```text
build\Debug\loxsort_fuzz_public_api.exe -runs=1000 fuzz\corpus\public_api
build\Debug\loxsort_fuzz_algorithms.exe -runs=1000 fuzz\corpus\algorithms
```

## Corpus policy

- Keep seed corpora in `fuzz/corpus/`.
- Do not commit crash artifacts, coverage output, or build directories.
- Update or extend corpus inputs when a new bug class is found.

## Safety notes

- Fuzzing is an engineering validation tool, not a release artifact.
- The checked-in corpus is public source material and may be used to reproduce regressions.
