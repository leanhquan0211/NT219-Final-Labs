Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

Set-Location "N:\UIT\HK2\NT219\Final_lab"

cmake -B build-mingw -S . `
  -G "Ninja" `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake `
  -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic

cmake --build build-mingw

Write-Host "Windows MinGW build completed."
