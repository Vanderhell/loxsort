# LoxSort — Test Strategy

## 1. Algorithm correctness

Každý algoritmus sa testuje priamo, nie iba cez dispatcher.

### Exhaustive small arrays

Pre hodnoty `{0,1,2,3}` otestovať všetky polia minimálne do `n = 8`, podľa rozumného rozdelenia testov. Výsledok sa porovná s referenčne zoradenou kópiou.

### Structured patterns

- empty;
- one element;
- two elements sorted/reversed/equal;
- sorted;
- reverse;
- all equal;
- alternating;
- few unique;
- nearly sorted;
- random;
- min/max key values;
- non-power-of-two counts.

### Element sizes

Testovať minimálne 1, 2, 3, 4, 7, 8, 16, 31 a 32 bajtov. Neštandardné veľkosti odhalia chybné alignment a block-copy predpoklady.

### Permutation

Nestačí overiť monotónnosť. Test musí potvrdiť, že výstup obsahuje presne vstupné records. Na hoste možno použiť unikátne record IDs a referenčný sort.

## 2. Stability

Tagged record:

```c
typedef struct {
    int key;
    uint32_t original_index;
} stable_record_t;
```

Comparator porovnáva iba `key`. Po stable sorte musia rovnaké keys zachovať rastúci `original_index`.

Povinné pre insertion a merge. Dispatcher testuje stabilitu pri `REQUIRE_STABLE` vo všetkých fallback vetvách.

## 3. Dispatcher tests

Povinné branch tests:

- trivial input;
- small input bez sampling;
- stable + scratch → merge;
- stable + nearly sorted + cutoff → insertion;
- stable bez scratch → insertion fallback;
- min writes + cycle eligible → cycle;
- min writes + duplicate-heavy → nie cycle;
- no recursion → nikdy introsort;
- nearly sorted pod max count → insertion;
- opportunistic merge profile range so scratch a bez scratch;
- Shell profile range;
- intro general path;
- disabled algorithms a fallback order;
- invalid profile → generic fallback;
- profile mask bez insertion → generic fallback alebo povinné doplnenie insertion podľa presného validator kontraktu.

## 4. Feature extractor tests

- `count <= small_count_cutoff`: extractor sa nevolá;
- maximálne 16 comparator calls;
- exact comparator count pre 1, 2, 3, 16 a veľké `count`;
- všetky indexy v rozsahu, overené guard pages alebo sanitizerom;
- sorted → disorder 0;
- reverse → vysoké disorder;
- all equal → equal 255;
- deterministický výstup;
- comparator vracajúci ľubovoľné záporné/kladné hodnoty, nie iba ±1.

## 5. API validation

Testovať:

- `count == 0` s NULL argumentmi podľa kontraktu;
- NULL base pri non-empty;
- zero element size;
- NULL comparator pri `count >= 2`;
- `count * element_size` overflow;
- invalid flags;
- scratch pointer/size nekonzistencia;
- result initialization pri error return;
- `lox_merge_scratch_required()` overflow.

## 6. Memory safety

Povinné host buildy:

```text
AddressSanitizer
UndefinedBehaviorSanitizer
```

Odporúčané:

```text
MemorySanitizer na podporovanej Clang platforme
Valgrind ako doplnok, nie náhrada sanitizerov
```

## 7. Fuzzing

Minimálne dva targets:

1. priame fuzzovanie každého algoritmu;
2. fuzzovanie verejného `lox_sort()` s flags, scratch availability a generated profile values.

Fuzzer musí:

- odvodiť count a element size z bounded inputu;
- používať total-order comparator;
- po sorte overiť monotónnosť aj permutation fingerprint/record IDs;
- pri stable flagu overiť stabilitu;
- bežať s ASan + UBSan.

Seed corpus obsahuje malé hraničné prípady, duplicates, reverse a adversarial patterns.

## 8. Profile generator tests

- rovnaký input vytvorí byte-identický header a report;
- neznámy algorithm ID je error;
- chýbajúci scenario je error pre validated mode;
- failed correctness row sa nikdy nepoužije ako winner;
- tie-break je deterministický;
- generated profile sa skompiluje;
- simulated dispatcher v Pythone a C dispatcher dávajú rovnaké rozhodnutia pre shared fixture matrix.

## 9. Build matrix

Minimálne CI:

- Linux GCC Debug;
- Linux GCC Release;
- Linux Clang Debug + ASan/UBSan;
- Windows MinGW alebo iný reálne C99-kompatibilný toolchain;
- cycle OFF a ON;
- merge OFF a ON;
- intro OFF fallback build;
- benchmark tools ON/OFF.

## 10. Quality gates

Pre každý commit implementačnej fázy:

- `git diff --check`;
- clean configure;
- build bez warningov;
- CTest 100 %;
- žiadne sanitizer findings;
- žiadne nové TODO/FIXME v produkčnom kóde;
- žiadne neoverené tvrdenie v PROJECT_STATE.

Pre release navyše:

- fuzz baseline podľa definovaného času alebo počtu executions;
- code-size report;
- profile report, ak sa distribuuje target profil;
- clean working tree.
