# EXEC 05 — Full Validation, Sanitizers and Fuzzing

## Objective

Doplň dôkazy korektnosti a memory safety podľa test strategy. Nepridávaj nové features.

## Required reading

- `docs/TEST_STRATEGY.md`
- `RELEASE_CHECKLIST.md`
- celý existujúci test suite

## Execute

1. Doplň exhaustive small-array testy pre každý algoritmus.
2. Doplň property matrix pre patterns, counts a element sizes.
3. Doplň tagged-record stability matrix.
4. Doplň API invalid/overflow tests.
5. Doplň dispatcher branch a profile fallback coverage.
6. Doplň feature out-of-range ochranu a exact comparison budget tests.
7. Vytvor dva libFuzzer targets podľa spec.
8. Vytvor seed corpus.
9. Spusti ASan + UBSan test suite.
10. Spusti fuzz smoke baseline s explicitne reportovaným počtom executions alebo trvaním, ktoré nástroj reálne vypíše.
11. Oprav všetky nálezy v scope. Neumlčuj sanitizer alebo fuzzer false-positive tvrdením bez reprodukovateľného dôkazu.
12. Aktualizuj PROJECT_STATE iba faktami potvrdenými príkazmi.

## Forbidden

- Žiadne znižovanie assertions alebo odstránenie testov s cieľom dosiahnuť zelený build.
- Žiadne suppression files bez zdokumentovaného externého toolchain problému.
- Žiadne performance redesigny.

## Required verification

- full CTest;
- ASan;
- UBSan;
- fuzz smoke pre oba targets;
- cycle OFF/ON build;
- `git diff --check`.

Použi povinný finálny formát z `00_INDEX.md`.
