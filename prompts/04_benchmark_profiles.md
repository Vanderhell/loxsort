# EXEC 04 — Benchmark Runner and Offline Profile Generator

## Objective

Vytvor korektný benchmark framework a deterministický profile generator bez ML frameworku.

## Required reading

- `docs/BENCHMARK_AND_PROFILES.md`
- `docs/ALGORITHM_POLICY.md`
- `docs/TEST_STRATEGY.md`

## Execute

1. Vytvor deterministické no-allocation data generators pre všetky required patterns.
2. Vytvor oddelený `dataset_seed` a `timing_repetition` model.
3. Vytvor benchmark runner, ktorý:
   - generuje master dataset mimo timing;
   - obnovuje identickú work copy pre každý algorithm/repetition;
   - meria iba sort;
   - po každom behu verifikuje correctness;
   - zapisuje raw CSV podľa spec.
4. Implementuj clock abstraction minimálne pre POSIX a Windows host.
5. Oddeľ timing build od instrumented build.
6. Implementuj validated summary generation.
7. Vytvor `tools/generate_profile.py` iba s Python standard library.
8. Implementuj deterministic threshold grid search vrátane Shell a opportunistic merge prahov a tie-break presne podľa spec.
9. Vygeneruj generic fixture profile, target profile header a Markdown report na malom test datasete.
10. Pridaj tests:
    - generator determinism;
    - same dataset pre všetky algoritmy;
    - timing repetition nemení dataset;
    - failed rows nie sú winner;
    - byte-identical profile output;
    - Python/C decision fixture parity.
11. Neoznač žiadny target profil ako validated, ak nebol reálne nameraný na identifikovanom targete a neprešiel gates.

## Forbidden

- Žiadny scikit-learn, pandas alebo numpy dependency.
- Žiadne runtime profile generation v knižnici.
- Žiadne meranie data generation/copy/verification ako sort time.
- Žiadne tvrdenie, že host profil platí pre MCU.

## Required verification

- benchmark tool build;
- deterministic generator tests;
- profile generator tests;
- smoke benchmark s correctness 100 %;
- `git diff --check`.

Použi povinný finálny formát z `00_INDEX.md`.
