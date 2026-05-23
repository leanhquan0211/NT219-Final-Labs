param(
    [Parameter(Mandatory=$true)]
    [string]$ProjectRoot,

    [string]$Python = "python"
)

$ErrorActionPreference = "Stop"

Set-Location $ProjectRoot

$InputDir = Join-Path $ProjectRoot "benchmarks\results"
$OutputDir = Join-Path $ProjectRoot "benchmarks\plots"
$ScriptPath = Join-Path $ProjectRoot "scripts\python\lab1_plot_benchmarks.py"

if (!(Test-Path $InputDir)) {
    throw "Benchmark result directory does not exist: $InputDir"
}

New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

& $Python $ScriptPath --input-dir $InputDir --output-dir $OutputDir
if ($LASTEXITCODE -ne 0) {
    throw "Plot generation failed with exit code ${LASTEXITCODE}"
}

Write-Host "Lab 1 plot generation completed."
