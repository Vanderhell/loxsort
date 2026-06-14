# Resume Verification Report

## Commands
- prefix: C:\Users\vande\Desktop\loxsort\build_release\Release\loxsort_bench.exe --campaign smoke --seed-base 1 --output C:\Users\vande\Desktop\loxsort\generated\resume_prefix.csv --checkpoint-interval 11 --stop-after-datasets 37 --timing-target-ms 2 --timing-samples 3
- resume: C:\Users\vande\Desktop\loxsort\build_release\Release\loxsort_bench.exe --campaign smoke --seed-base 1 --output C:\Users\vande\Desktop\loxsort\generated\resume_prefix.csv --checkpoint-interval 11 --resume --timing-target-ms 2 --timing-samples 3
- uninterrupted: C:\Users\vande\Desktop\loxsort\build_release\Release\loxsort_bench.exe --campaign smoke --seed-base 1 --output C:\Users\vande\Desktop\loxsort\generated\resume_full.csv --checkpoint-interval 11 --timing-target-ms 2 --timing-samples 3

## Validation
- prefix return code: 0
- resumed return code: 0
- uninterrupted return code: 0
- resumed validator: 0
- uninterrupted validator: 0

## Comparison
- resumed row count: 700
- uninterrupted row count: 700
- resumed dataset count: 100
- uninterrupted dataset count: 100
- duplicate dataset ids in resumed output: none
- missing dataset ids in resumed output: none
- extra dataset ids in resumed output: none
- strategy mismatch datasets: none

## Validator Output
```
validated_rows=700
validated_datasets=100
campaign_ids=['smoke']
families=['characters', 'floating_point', 'integers', 'records', 'strings', 'timestamps']
merge_eligible_datasets=54

validated_rows=700
validated_datasets=100
campaign_ids=['smoke']
families=['characters', 'floating_point', 'integers', 'records', 'strings', 'timestamps']
merge_eligible_datasets=54

```

## Scenario Digest
campaign_ids=['smoke']
