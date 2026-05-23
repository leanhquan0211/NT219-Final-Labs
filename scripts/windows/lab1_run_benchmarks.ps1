param(
    [string]$ProjectRoot = (Resolve-Path ".").Path,
    [string]$Configuration = "mingw",
    [int]$Runs = 30
)

$ErrorActionPreference = "Stop"

$ProjectRoot = (Resolve-Path $ProjectRoot).Path
Set-Location $ProjectRoot

$binDir = if ($Configuration -eq "msvc") { "build-msvc\bin" } else { "build-mingw\bin" }
$aesTool = Join-Path $ProjectRoot "$binDir\aestool.exe"

if (-not (Test-Path $aesTool)) {
    throw "Cannot find aestool at $aesTool"
}

$resultsDir = Join-Path $ProjectRoot "benchmarks\results"
New-Item -ItemType Directory -Force -Path $resultsDir | Out-Null

$keyPath = Join-Path $ProjectRoot "samples\keys\lab1_bench_aes256.key"
New-Item -ItemType Directory -Force -Path (Split-Path $keyPath -Parent) | Out-Null

& $aesTool keygen --size 32 --out $keyPath
if ($LASTEXITCODE -ne 0) {
    throw "aestool keygen failed with exit code ${LASTEXITCODE}"
}

$timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
$outCsv = Join-Path $resultsDir "lab1_${Configuration}_${timestamp}.csv"

"mode,size_bytes,runs,mean_ms,median_ms,stddev_ms,ci95_low_ms,ci95_high_ms,throughput_mib_s" | Set-Content -Encoding UTF8 $outCsv

$modes = @("cbc", "ctr", "gcm", "ccm")
$sizes = @(1024, 4096, 16384, 262144, 1048576, 8388608)

foreach ($mode in $modes) {
    foreach ($size in $sizes) {
        Write-Host "Benchmarking mode=$mode size=$size runs=$Runs"
        $output = & $aesTool bench --mode $mode --key $keyPath --size $size --runs $Runs
        if ($LASTEXITCODE -ne 0) {
            throw "aestool bench failed for mode=$mode size=$size with exit code ${LASTEXITCODE}"
        }

        $lines = $output -split "`r?`n" | Where-Object { $_.Trim().Length -gt 0 }
        if ($lines.Count -lt 2) {
            throw "Unexpected benchmark output for mode=$mode size=$size"
        }

        Add-Content -Encoding UTF8 -Path $outCsv -Value $lines[-1]
    }
}

Write-Host "Benchmark CSV written to:"
Write-Host $outCsv
