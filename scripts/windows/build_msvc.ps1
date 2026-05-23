Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

Set-Location "N:\UIT\HK2\NT219\Final_lab"

cmake -B build-msvc -S . `
  -G "Ninja" `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake `
  -DVCPKG_TARGET_TRIPLET=x64-windows

cmake --build build-msvc

Write-Host "Windows MSVC build completed."
