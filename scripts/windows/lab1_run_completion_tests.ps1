param(
    [string]$ProjectRoot = "N:\UIT\HK2\NT219\Final_lab",
    [string]$Configuration = "mingw"
)

$ErrorActionPreference = "Stop"

function Invoke-CheckedCommand {
    param([string]$FilePath, [string[]]$Arguments)
    & $FilePath @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "Command failed with exit code ${LASTEXITCODE}: $FilePath $($Arguments -join ' ')"
    }
}

Set-Location $ProjectRoot

$BinDir = if ($Configuration -eq "msvc") { "build-msvc\bin" } else { "build-mingw\bin" }
$AesTool = Join-Path $ProjectRoot "$BinDir\aestool.exe"
$SampleDir = Join-Path $ProjectRoot "samples"
$KeyDir = Join-Path $ProjectRoot "samples\keys"
New-Item -ItemType Directory -Force -Path $SampleDir | Out-Null
New-Item -ItemType Directory -Force -Path $KeyDir | Out-Null

Write-Host "Running Lab 1 completion tests with $AesTool"

# XTS round trip
$XtsKey = Join-Path $KeyDir "lab1_completion_xts.key"
$XtsPlain = Join-Path $SampleDir "lab1_completion_xts_plain.bin"
$XtsCt = Join-Path $SampleDir "lab1_completion_xts.ct"
$XtsRecovered = Join-Path $SampleDir "lab1_completion_xts_recovered.bin"
$XtsMeta = Join-Path $SampleDir "lab1_completion_xts.json"

Invoke-CheckedCommand $AesTool @("keygen", "--size", "32", "--out", $XtsKey)
$plainBytes = New-Object byte[] 64
for ($i = 0; $i -lt $plainBytes.Length; $i++) { $plainBytes[$i] = [byte]$i }
[System.IO.File]::WriteAllBytes($XtsPlain, $plainBytes)
Invoke-CheckedCommand $AesTool @("encrypt", "--mode", "xts", "--key", $XtsKey, "--iv-hex", "000102030405060708090a0b0c0d0e0f", "--in", $XtsPlain, "--out", $XtsCt, "--meta", $XtsMeta)
Invoke-CheckedCommand $AesTool @("decrypt", "--mode", "xts", "--key", $XtsKey, "--meta", $XtsMeta, "--in", $XtsCt, "--out", $XtsRecovered)
$original = [System.IO.File]::ReadAllBytes($XtsPlain)
$recovered = [System.IO.File]::ReadAllBytes($XtsRecovered)
if (-not [System.Linq.Enumerable]::SequenceEqual($original, $recovered)) { throw "AES-XTS round trip failed" }
Write-Host "PASS: AES-XTS round trip succeeded."

# Nonce reuse tracking
$GcmKey = Join-Path $KeyDir "lab1_completion_gcm.key"
$NonceDb = Join-Path $SampleDir "lab1_completion_nonce_db.json"
$GcmCt1 = Join-Path $SampleDir "lab1_completion_gcm_1.ct"
$GcmCt2 = Join-Path $SampleDir "lab1_completion_gcm_2.ct"
Remove-Item $NonceDb -Force -ErrorAction SilentlyContinue
Invoke-CheckedCommand $AesTool @("keygen", "--size", "32", "--out", $GcmKey)
Invoke-CheckedCommand $AesTool @("encrypt", "--mode", "gcm", "--key", $GcmKey, "--nonce-hex", "000102030405060708090a0b", "--text", "nonce test", "--out", $GcmCt1, "--nonce-db", $NonceDb)
& $AesTool encrypt --mode gcm --key $GcmKey --nonce-hex 000102030405060708090a0b --text "nonce test again" --out $GcmCt2 --nonce-db $NonceDb
if ($LASTEXITCODE -eq 0) { throw "Nonce reuse was not rejected" }
Write-Host "PASS: AES-GCM repeated key+nonce was rejected by nonce DB."

# Final KAT
Invoke-CheckedCommand $AesTool @("kat", "--kat", "vectors\lab1\aes_stage3e_final_kat_vectors.json")

Write-Host "All Lab 1 completion tests passed."
