# Release Notes

## v1.0.0

Release date: 2026-07-17

This is the first public release tagged in the repository.

### Highlights

- MIT-licensed public source release.
- Deterministic C99 sorting library for embedded and resource-constrained systems.
- Stateless API with explicit sort options and result reporting.
- Dispatcher coverage for insertion, Shell, introsort, merge sort, and optional cycle sort.
- Public documentation set for API usage, cookbook guidance, fuzzing, contributing, and release notes.
- CI workflow and release workflow present in the repository.

### Verification summary

- Full `1,000,000`-dataset campaign completed in the workspace.
- Final release report generated from the completed benchmark CSV.
- The dispatcher is correct and measurable at scale.
- The dispatcher is not globally optimal against the oracle and fixed-strategy baselines on every aggregated view.

### Practical note

LoxSort is intended as an experimental, auditable runtime for constrained environments, not as a universal benchmark winner.
