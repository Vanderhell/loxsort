# EXEC 06 — Professional Build, CI and Release Baseline

## Objective

Dokonči profesionálny build, CI, packaging a pravdivú dokumentáciu v0.1 bez pridávania funkcionality.

## Required reading

- všetky docs;
- `PROJECT_STATE.md`;
- `RELEASE_CHECKLIST.md`;
- aktuálne build/test výsledky.

## Execute

1. Dokonči CMake options podľa master spec.
2. Zabezpeč oddelené targets:
   - `loxsort` library;
   - tests;
   - benchmark;
   - fuzz targets, ak toolchain podporuje.
3. Pridaj install/export package pre public header a library.
4. Pridaj CTest integráciu.
5. Pridaj warning policy bez vnucovania `-Werror` downstream konzumentom.
6. Pridaj GitHub Actions matrix podľa test strategy.
7. Pridaj code-size report job alebo lokálny target.
8. Aktualizuj README iba podľa reálne implementovaného a overeného API.
9. Aktualizuj PROJECT_STATE a RELEASE_CHECKLIST pravdivo.
10. Over clean build z nového adresára v Debug aj Release.
11. Over install a minimálny external consumer example.
12. Ak všetky povinné gates prejdú, vytvor jeden commit s vecným subjectom. Nepridávaj agenta ako Author/CoAuthor a nepridávaj `Co-authored-by` trailer.

## Forbidden

- Žiadne nové algoritmy alebo dispatcher rules.
- Žiadne badge na neexistujúci workflow.
- Žiadne „production ready“ tvrdenie bez splneného checklistu.
- Žiadny tag, ak release gates neprešli.

## Required verification

- Debug clean configure/build/test;
- Release clean configure/build/test;
- sanitizer build/test;
- install + external consumer build/run;
- CI syntax/config review;
- clean working tree po commite, ak commit vznikol;
- `git diff --check` pred commitom.

Použi povinný finálny formát z `00_INDEX.md`.
