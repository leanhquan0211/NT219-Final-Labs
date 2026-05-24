param(
    [Parameter(Mandatory=$true)]
    [string]$ProjectRoot
)

$ErrorActionPreference = "Stop"
$ProjectRoot = (Resolve-Path $ProjectRoot).Path
$GuiRoot = Join-Path $ProjectRoot "gui\lab1_python"
$NativeRoot = Join-Path $GuiRoot "native"
$MingwNative = Join-Path $NativeRoot "mingw"
$MsvcNative = Join-Path $NativeRoot "msvc"
$LinuxNative = Join-Path $NativeRoot "linux"

New-Item -ItemType Directory -Force $MingwNative | Out-Null
New-Item -ItemType Directory -Force $MsvcNative | Out-Null
New-Item -ItemType Directory -Force $LinuxNative | Out-Null

function Copy-FirstExisting {
    param(
        [string[]]$Candidates,
        [string]$Destination
    )
    foreach ($candidate in $Candidates) {
        if (Test-Path $candidate) {
            Copy-Item $candidate $Destination -Force
            Write-Host "Copied $candidate -> $Destination"
            return $true
        }
    }
    return $false
}

Copy-FirstExisting @(
    (Join-Path $ProjectRoot "build-mingw\bin\libnt219_lab1_aes_shared.dll"),
    (Join-Path $ProjectRoot "bin\windows-mingw\libnt219_lab1_aes_shared.dll")
) $MingwNative | Out-Null

Copy-FirstExisting @(
    (Join-Path $ProjectRoot "build-msvc\bin\nt219_lab1_aes_shared.dll"),
    (Join-Path $ProjectRoot "build-msvc\bin\libnt219_lab1_aes_shared.dll"),
    (Join-Path $ProjectRoot "bin\windows-msvc\nt219_lab1_aes_shared.dll"),
    (Join-Path $ProjectRoot "bin\windows-msvc\libnt219_lab1_aes_shared.dll")
) $MsvcNative | Out-Null

Copy-FirstExisting @(
    (Join-Path $ProjectRoot "build-linux\lab1_aes_cryptopp\libnt219_lab1_aes_shared.so"),
    (Join-Path $ProjectRoot "build-linux\bin\libnt219_lab1_aes_shared.so"),
    (Join-Path $ProjectRoot "bin\linux\libnt219_lab1_aes_shared.so")
) $LinuxNative | Out-Null

foreach ($dir in @("build-mingw\bin", "bin\windows-mingw")) {
    $full = Join-Path $ProjectRoot $dir
    if (Test-Path $full) {
        Get-ChildItem $full -Filter "*.dll" -ErrorAction SilentlyContinue | ForEach-Object {
            Copy-Item $_.FullName $MingwNative -Force
        }
    }
}

foreach ($dir in @("build-msvc\bin", "bin\windows-msvc")) {
    $full = Join-Path $ProjectRoot $dir
    if (Test-Path $full) {
        Get-ChildItem $full -Filter "*.dll" -ErrorAction SilentlyContinue | ForEach-Object {
            Copy-Item $_.FullName $MsvcNative -Force
        }
    }
}

$MsysBin = "C:\msys64\mingw64\bin"
foreach ($runtime in @("libstdc++-6.dll", "libgcc_s_seh-1.dll", "libwinpthread-1.dll")) {
    $p = Join-Path $MsysBin $runtime
    if (Test-Path $p) {
        Copy-Item $p $MingwNative -Force
        Write-Host "Copied MinGW runtime $runtime"
    }
}

Write-Host "Native libraries collected under $NativeRoot"
