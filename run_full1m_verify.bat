@echo off
setlocal EnableExtensions EnableDelayedExpansion

set "ROOT=%~dp0"
set "EXE=%ROOT%build_release\Release\loxsort_bench.exe"
set "CSV=%ROOT%generated\full1m_bench_release.csv"
set "REPORT=%ROOT%generated\full1m_report_release.md"
set "OUTLOG=%ROOT%generated\full1m_bench_release.out.log"
set "ERRLOG=%ROOT%generated\full1m_bench_release.err.log"
set "CHECKPOINT=%ROOT%generated\benchmark.checkpoint.json"

if not exist "%EXE%" (
    echo missing benchmark executable: "%EXE%"
    exit /b 1
)

if not exist "%ROOT%generated" mkdir "%ROOT%generated"

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$exe = '%EXE%';" ^
  "$csv = '%CSV%';" ^
  "$report = '%REPORT%';" ^
  "$out = '%OUTLOG%';" ^
  "$err = '%ERRLOG%';" ^
  "$checkpoint = '%CHECKPOINT%';" ^
  "$args = @('--campaign','full-1m','--seed-base','1','--output',$csv,'--report',$report,'--checkpoint-interval','1000','--timing-target-ms','2','--timing-samples','3');" ^
  "$p = Start-Process -FilePath $exe -ArgumentList $args -WindowStyle Hidden -RedirectStandardOutput $out -RedirectStandardError $err -PassThru;" ^
  "Write-Host ('phase=full-1m verification; started pid={0}' -f $p.Id);" ^
  "while (-not $p.HasExited) {" ^
  "  $lines = if (Test-Path $csv) { (Get-Content $csv | Measure-Object -Line).Lines } else { 0 };" ^
  "  $cp = if (Test-Path $checkpoint) { 'checkpoint=yes' } else { 'checkpoint=no' };" ^
  "  try { $cpu = (Get-Process -Id $p.Id).CPU } catch { $cpu = 0 };" ^
  "  Write-Host ('phase=full-1m verification; pid={0}; csv_lines={1}; {2}; cpu={3:N1}s' -f $p.Id, $lines, $cp, $cpu);" ^
  "  Start-Sleep -Seconds 30;" ^
  "  $p.Refresh();" ^
  "}" ^
  "Write-Host ('phase=full-1m verification; status=done; exit_code={0}' -f $p.ExitCode);" ^
  "exit $p.ExitCode"

if errorlevel 1 exit /b %errorlevel%

echo validating full1m CSV
python "%ROOT%tools\validate_benchmark.py" "%CSV%"
if errorlevel 1 exit /b %errorlevel%

echo generating full1m report
python "%ROOT%tools\generate_benchmark_report.py" "%CSV%" "%REPORT%"
if errorlevel 1 exit /b %errorlevel%

echo done
exit /b 0
