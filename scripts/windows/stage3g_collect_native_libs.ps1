param(
    [string]$ProjectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..\..")).Path
)

$ErrorActionPreference = "Continue"
$ProjectRoot = (Resolve-Path $ProjectRoot).Path
$GuiDir = Join-Path $ProjectRoot "gui\lab1_python"
$NativeDir = Join-Path $GuiDir "native"
New-Item -ItemType Directory -Force (Join-Path $NativeDir "mingw") | Out-Null
New-Item -ItemType Directory -Force (Join-Path $NativeDir "msvc") | Out-Null
New-Item -ItemType Directory -Force (Join-Path $NativeDir "linux") | Out-Null

function Copy-IfExists($Pattern, $Destination) {
    $files = Get-ChildItem $ProjectRoot -Recurse -File -ErrorAction SilentlyContinue | Where-Object { $_.FullName -like $Pattern }
    foreach ($f in $files) {
        Copy-Item $f.FullName $Destination -Force
        Write-Host "Copied $($f.FullName) -> $Destination"
    }
}

$mingwDest = Join-Path $NativeDir "mingw"
$msvcDest = Join-Path $NativeDir "msvc"
$linuxDest = Join-Path $NativeDir "linux"

Copy-IfExists "*build-mingw*bin*lab1*aes*shared*.dll" $mingwDest
Copy-IfExists "*build-mingw*bin*libcrypto-3-x64.dll" $mingwDest
Copy-IfExists "*build-mingw*bin*libgcc*.dll" $mingwDest
Copy-IfExists "*build-mingw*bin*libstdc++*.dll" $mingwDest
Copy-IfExists "*build-mingw*bin*libwinpthread*.dll" $mingwDest

Copy-IfExists "*build-msvc*bin*lab1*aes*shared*.dll" $msvcDest
Copy-IfExists "*build-msvc*bin*libcrypto-3-x64.dll" $msvcDest

Copy-IfExists "*build-linux*lab1_aes_cryptopp*libnt219_lab1_aes_shared.so" $linuxDest
Copy-IfExists "*build-linux*bin*libnt219_lab1_aes_shared.so" $linuxDest

Write-Host "Native libraries collected under: $NativeDir"
Write-Host "Note: Windows Python can load MinGW/MSVC .dll only. Linux .so is for Linux/WSL Python."
