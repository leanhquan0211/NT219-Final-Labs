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

$key32Path = Join-Path $ProjectRoot "samples\keys\lab1_bench_aes256.key"
$key64Path = Join-Path $ProjectRoot "samples\keys\lab1_bench_xts_aes256x2.key"
New-Item -ItemType Directory -Force -Path (Split-Path $key32Path -Parent) | Out-Null

& $aesTool keygen --size 32 --out $key32Path
if ($LASTEXITCODE -ne 0) { throw "aestool AES-256 keygen failed with exit code ${LASTEXITCODE}" }

& $aesTool keygen --size 64 --out $key64Path
if ($LASTEXITCODE -ne 0) { throw "aestool AES-XTS 64-byte keygen failed with exit code ${LASTEXITCODE}" }

$timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
$outCsv = Join-Path $resultsDir "lab1_${Configuration}_${timestamp}.csv"

"environment,mode,size_bytes,runs,mean_ms,median_ms,stddev_ms,ci95_low_ms,ci95_high_ms,throughput_mib_s" | Set-Content -Encoding UTF8 $outCsv

$modes = @("ecb", "cbc", "cfb", "ofb", "ctr", "xts", "ccm", "gcm")
$sizes = @(1024, 4096, 16384, 262144, 1048576, 8388608)

foreach ($mode in $modes) {
    foreach ($size in $sizes) {
        Write-Host "Benchmarking env=$Configuration mode=$mode size=$size runs=$Runs"
        $keyPath = if ($mode -eq "xts") { $key64Path } else { $key32Path }

        $extra = @()
        if ($mode -eq "ecb") {
            $extra += "--allow-ecb"
        }

        $output = & $aesTool bench --mode $mode --key $keyPath --size $size --runs $Runs @extra
        if ($LASTEXITCODE -ne 0) {
            throw "aestool bench failed for mode=$mode size=$size with exit code ${LASTEXITCODE}"
        }

        $lines = $output -split "`r?`n" | Where-Object { $_.Trim().Length -gt 0 }
        if ($lines.Count -lt 2) {
            throw "Unexpected benchmark output for mode=$mode size=$size"
        }

        $row = $lines[-1]
        if ($row -match "^[a-zA-Z]+,") {
            Add-Content -Encoding UTF8 -Path $outCsv -Value "$Configuration,$row"
        } else {
            throw "Unexpected benchmark row format: $row"
        }
    }
}

Write-Host "Benchmark CSV written to:"
Write-Host $outCsv
