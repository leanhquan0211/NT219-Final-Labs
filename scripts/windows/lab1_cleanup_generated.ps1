param(
    [Parameter(Mandatory=$true)]
    [string]$ProjectRoot
)

$ErrorActionPreference = "Stop"

Set-Location $ProjectRoot

$patterns = @(
    "samples\lab1_*.ct",
    "samples\lab1_*.json",
    "samples\lab1_*_out.txt",
    "samples\lab1_*_recovered.txt",
    "samples\lab1_*_large.bin",
    "samples\lab1_negative_*.bin",
    "samples\lab1_negative_*.ct",
    "samples\lab1_negative_*.json",
    "samples\lab1_negative_*.txt"
)

foreach ($pattern in $patterns) {
    Remove-Item $pattern -Force -ErrorAction SilentlyContinue
}

Write-Host "Generated Lab 1 local test files cleaned."
