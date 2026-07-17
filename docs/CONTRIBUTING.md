# Contributing to LoxSort

Contributions are welcome when they preserve the library's deterministic, constraint-aware design.

## Before you open a pull request

- Build the project in both Debug and Release.
- Run the test suite.
- Verify benchmark or profile changes against the existing documentation.
- Keep generated artifacts out of source changes unless they are intentionally updated.
- Update the relevant documentation when behavior changes.

## Good change scope

Prefer changes that are:

- narrowly scoped;
- documented;
- covered by tests;
- deterministic across toolchains where practical.

## Bug reports

Please include:

- target platform and compiler;
- build configuration and enabled CMake options;
- minimal reproduction steps;
- expected vs actual behavior;
- any benchmark CSV or report excerpt that supports the claim.

The GitHub issue tracker is the canonical place for bugs and feature requests:

https://github.com/Vanderhell/loxsort/issues
