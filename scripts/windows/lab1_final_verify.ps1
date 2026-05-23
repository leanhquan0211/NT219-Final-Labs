param(
    [Parameter(Mandatory=$true)]
    [string]$ProjectRoot,

    [ValidateSet("mingw", "msvc")]
    [string]$Configuration = "mingw",

    [int]$BenchmarkRuns = 5
)

$ErrorActionPreference = "Stop"

function Invoke-Checked {
    param(
        [Parameter(Mandatory=$true)]
        [string]$FilePath,

        [string[]]$Arguments = @(),

        [switch]$ExpectFailure
    )

    Write-Host ""
    Write-Host ">>> $FilePath $($Arguments -join ' ')"

    & $FilePath @Arguments
    $exitCode = $LASTEXITCODE

    if ($ExpectFailure) {
        if ($exitCode -eq 0) {
            throw "Expected command to fail but it succeeded: $FilePath $($Arguments -join ' ')"
        }
        Write-Host "Expected failure observed. Exit code: $exitCode"
        return
    }

    if ($exitCode -ne 0) {
        throw "Command failed with exit code ${exitCode}: $FilePath $($Arguments -join ' ')"
    }
}

Set-Location $ProjectRoot

$BuildDir = if ($Configuration -eq "msvc") { "build-msvc" } else { "build-mingw" }
$AesTool = Join-Path $ProjectRoot "$BuildDir\bin\aestool.exe"

if (!(Test-Path $AesTool)) {
    throw "aestool not found: $AesTool"
}

$SummaryDir = Join-Path $ProjectRoot "report\lab1"
New-Item -ItemType Directory -Force -Path $SummaryDir | Out-Null

$FinalCheckPath = Join-Path $SummaryDir "lab1_final_verification_log.md"
$Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"

@"
# Lab 1 Final Verification Log

Generated at: $Timestamp

Configuration: $Configuration

This log records final local verification commands for Lab 1 AES Crypto++.
"@ | Set-Content -Path $FinalCheckPath -Encoding UTF8

function Add-Log {
    param([string]$Text)
    Add-Content -Path $FinalCheckPath -Value $Text -Encoding UTF8
}

Add-Log "`n## Core checks`n"

Invoke-Checked $AesTool @("selftest")
Add-Log "- aestool selftest: PASS"

Invoke-Checked $AesTool @("kat", "--kat", "vectors\lab1\aes_stage3b_kat_vectors.json")
Add-Log "- KAT smoke set: PASS"

Add-Log "`n## Functional round-trip checks`n"

$KeyFile = Join-Path $ProjectRoot "samples\keys\lab1_final_aes256.key"
$GcmCt = Join-Path $ProjectRoot "samples\lab1_final_gcm.ct"
$GcmMeta = Join-Path $ProjectRoot "samples\lab1_final_gcm.json"
$GcmOut = Join-Path $ProjectRoot "samples\lab1_final_gcm_recovered.txt"

New-Item -ItemType Directory -Force -Path (Split-Path -Parent $KeyFile) | Out-Null

Invoke-Checked $AesTool @("keygen", "--size", "32", "--out", $KeyFile)
Invoke-Checked $AesTool @(
    "encrypt",
    "--mode", "gcm",
    "--key", $KeyFile,
    "--text", "Lab1 final verification",
    "--aad-text", "stage3d",
    "--out", $GcmCt,
    "--meta", $GcmMeta
)
Invoke-Checked $AesTool @(
    "decrypt",
    "--mode", "gcm",
    "--key", $KeyFile,
    "--in", $GcmCt,
    "--aad-text", "stage3d",
    "--meta", $GcmMeta,
    "--out", $GcmOut
)

$Recovered = Get-Content $GcmOut -Raw
if ($Recovered -ne "Lab1 final verification") {
    throw "GCM recovered text mismatch."
}

Add-Log "- AES-256-GCM encrypt/decrypt round-trip: PASS"

Add-Log "`n## Negative checks`n"

$TamperedCt = Join-Path $ProjectRoot "samples\lab1_final_gcm_tampered.ct"
Copy-Item $GcmCt $TamperedCt -Force
$bytes = [System.IO.File]::ReadAllBytes($TamperedCt)
$bytes[0] = $bytes[0] -bxor 1
[System.IO.File]::WriteAllBytes($TamperedCt, $bytes)

Invoke-Checked $AesTool @(
    "decrypt",
    "--mode", "gcm",
    "--key", $KeyFile,
    "--in", $TamperedCt,
    "--aad-text", "stage3d",
    "--meta", $GcmMeta,
    "--out", (Join-Path $ProjectRoot "samples\lab1_final_tampered_out.txt")
) -ExpectFailure

Add-Log "- GCM tampered ciphertext rejected: PASS"

$EcbKey = Join-Path $ProjectRoot "samples\keys\lab1_final_ecb.key"
$EcbLarge = Join-Path $ProjectRoot "samples\lab1_final_ecb_large.bin"
$EcbCt = Join-Path $ProjectRoot "samples\lab1_final_ecb_large.ct"

Invoke-Checked $AesTool @("keygen", "--size", "16", "--out", $EcbKey)

$largeBytes = New-Object byte[] 20000
[System.IO.File]::WriteAllBytes($EcbLarge, $largeBytes)

Invoke-Checked $AesTool @(
    "encrypt",
    "--mode", "ecb",
    "--key", $EcbKey,
    "--in", $EcbLarge,
    "--out", $EcbCt
) -ExpectFailure

Add-Log "- ECB input larger than 16 KiB blocked without --allow-ecb: PASS"

Add-Log "`n## Benchmark smoke`n"
Add-Log "- Benchmark runs requested for final verification: $BenchmarkRuns"
Add-Log "- See benchmarks/results and benchmarks/plots for generated local artifacts."

Add-Log "`n## Known limitations still tracked`n"
Add-Log "- XTS mode is not yet implemented in the current foundation."
Add-Log "- Full no-padding NIST CBC/CFB KAT path is tracked for a later hardening patch."
Add-Log "- CTR/GCM/CCM nonce reuse persistence database is tracked for a later hardening patch."

Write-Host ""
Write-Host "Lab 1 final verification log written to:"
Write-Host $FinalCheckPath
