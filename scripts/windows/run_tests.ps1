Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

Set-Location "N:\UIT\HK2\NT219\Final_lab"

ctest --test-dir build-mingw --output-on-failure

Write-Host "Windows MinGW tests completed."
