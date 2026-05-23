param(
    [string]$ProjectRoot = (Resolve-Path ".").Path,
    [string]$Configuration = "mingw"
)

$ErrorActionPreference = "Stop"

$ProjectRoot = (Resolve-Path $ProjectRoot).Path
Set-Location $ProjectRoot

$binDir = if ($Configuration -eq "msvc") { "build-msvc\bin" } else { "build-mingw\bin" }
$aesTool = Join-Path $ProjectRoot "$binDir\aestool.exe"

if (-not (Test-Path $aesTool)) {
    throw "Cannot find aestool at $aesTool"
}

New-Item -ItemType Directory -Force -Path (Join-Path $ProjectRoot "samples\keys") | Out-Null

$keyFile = Join-Path $ProjectRoot "samples\keys\lab1_negative_aes256.key"
$ctFile = Join-Path $ProjectRoot "samples\lab1_negative_gcm.ct"
$metaFile = Join-Path $ProjectRoot "samples\lab1_negative_gcm.json"
$tamperedFile = Join-Path $ProjectRoot "samples\lab1_negative_gcm_tampered.ct"
$outFile = Join-Path $ProjectRoot "samples\lab1_negative_out.txt"

& $aesTool keygen --size 32 --out $keyFile
if ($LASTEXITCODE -ne 0) {
    throw "keygen failed"
}

& $aesTool encrypt --mode gcm --key $keyFile --text "negative-test-message" --aad-text "metadata" --out $ctFile --meta $metaFile
if ($LASTEXITCODE -ne 0) {
    throw "gcm encrypt failed"
}

Copy-Item $ctFile $tamperedFile -Force
$bytes = [System.IO.File]::ReadAllBytes($tamperedFile)
$bytes[0] = $bytes[0] -bxor 1
[System.IO.File]::WriteAllBytes($tamperedFile, $bytes)

& $aesTool decrypt --mode gcm --key $keyFile --in $tamperedFile --aad-text "metadata" --meta $metaFile --out $outFile
if ($LASTEXITCODE -eq 0) {
    throw "GCM tampered ciphertext was accepted. This is a security failure."
}

Write-Host "PASS: GCM tampered ciphertext was rejected."

$ecbKey = Join-Path $ProjectRoot "samples\keys\lab1_negative_ecb.key"
$largeFile = Join-Path $ProjectRoot "samples\lab1_negative_ecb_large.bin"
$ecbOut = Join-Path $ProjectRoot "samples\lab1_negative_ecb_large.ct"

& $aesTool keygen --size 16 --out $ecbKey
if ($LASTEXITCODE -ne 0) {
    throw "ecb keygen failed"
}

$largeBytes = New-Object byte[] 20000
[System.IO.File]::WriteAllBytes($largeFile, $largeBytes)

& $aesTool encrypt --mode ecb --key $ecbKey --in $largeFile --out $ecbOut
if ($LASTEXITCODE -eq 0) {
    throw "ECB large input was accepted without --allow-ecb. This is a policy failure."
}

Write-Host "PASS: ECB large input was blocked without --allow-ecb."
Write-Host "All Lab 1 negative tests passed."
