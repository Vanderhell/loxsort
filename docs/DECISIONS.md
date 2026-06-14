# LoxSort — Locked Design Decisions

## D-001 — Žiadny runtime learning

**Decision:** v0.1 používa offline benchmark profile.  
**Reason:** runtime benchmarking by pri malých a stredných poliach stálo viac než sort a menilo používateľské dáta alebo vyžadovalo plné kópie.

## D-002 — Malé vstupy bez analýzy

**Decision:** `count <= small_count_cutoff` ide priamo na insertion sort.  
**Reason:** fixná analytická réžia je pri malých poliach neobhájiteľná.

## D-003 — Maximálne 16 porovnaní

**Decision:** feature extractor analyzuje iba rovnomerne rozložené susedné páry.  
**Reason:** pevný rozpočet, bezpečné indexovanie a nízka réžia.

## D-004 — Stateless public API

**Decision:** žiadny `lox_init`, `lox_ctx_t` ani default mutable context.  
**Reason:** menšie API, reentrancy a odstránenie neplatného opaque stack allocation kontraktu.

## D-005 — Explicitný registry

**Decision:** algoritmy sa registrujú explicitným compile-time poľom.  
**Reason:** ISO C99 portabilita; žiadne linker sections ani GCC attributes.

## D-006 — Zúžený katalóg

**Decision:** insertion, Shell, introsort, merge a optional cycle.  
**Reason:** každý algoritmus má odlišnú oprávnenú rolu. Duplicitné alebo slabé varianty zvyšujú code size, testovací priestor a classifier noise.

## D-007 — Timsort odstránený

**Decision:** Timsort nie je vo v0.1.  
**Reason:** vysoká implementačná a validačná zložitosť bez jasného embedded prínosu oproti insertion + merge kombinácii.

## D-008 — Heapsort iba interne

**Decision:** heapsort je fallback introsortu, nie verejne vyberaná stratégia.  
**Reason:** zachová worst-case O(n log n) bez ďalšej dispatcher vetvy.

## D-009 — Stabilný no-scratch fallback

**Decision:** pri požadovanej stabilite a nedostatočnom scratch sa použije insertion sort.  
**Reason:** stabilita je hard constraint a korektný O(1) stable fallback je hodnotnejší než error. Result reason musí túto potenciálne pomalú cestu priznať.

## D-010 — Cycle sort predvolene vypnutý

**Decision:** cycle je optional build feature.  
**Reason:** špecializovaný use case, O(n²) comparisons a vyššie riziko chýb pri duplicitách.

## D-011 — Žiadny ML framework

**Decision:** profile generator používa deterministický threshold search.  
**Reason:** výstup je auditovateľný, malý, reprodukovateľný a bez zbytočnej Python dependency vrstvy.

## D-012 — Platformová platnosť profilov

**Decision:** profil je viazaný na target, compiler a flags.  
**Reason:** relatívny výkon sortov sa medzi x86, ESP32, STM32 a AVR zásadne mení.

## D-013 — Merge môže byť aj výkonová cesta

**Decision:** pri dostatočnom scratch môže target profil vybrať merge aj bez požiadavky stability.  
**Reason:** benchmark môže na konkrétnom targete a pri konkrétnej veľkosti recordov preukázať výhodu merge sortu. Generic profil môže túto cestu konzervatívne vypnúť nulovým prahom.
