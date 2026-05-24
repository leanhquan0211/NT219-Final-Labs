param(
    [string]$ProjectRoot = (Resolve-Path ".").Path
)

$ErrorActionPreference = "Stop"
$ProjectRoot = (Resolve-Path $ProjectRoot).Path
Set-Location $ProjectRoot

New-Item -ItemType Directory -Force report\figures\lab1 | Out-Null
New-Item -ItemType Directory -Force report\tables\lab1 | Out-Null

python scripts\python\lab1_generate_environment_plots.py `
    --input-dir report\lab1\benchmark_results `
    --output-dir report\figures\lab1 `
    --tables-dir report\tables\lab1

Copy-Item report\figures\lab1\lab1_aes_*_by_environment.png report\lab1\benchmark_plots\ -Force -ErrorAction SilentlyContinue
Write-Host "Stage 3G environment plots and summary tables generated."
