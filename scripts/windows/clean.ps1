Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

Set-Location "N:\UIT\HK2\NT219\Final_lab"

Remove-Item -Recurse -Force build-mingw -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force build-msvc -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force build-linux -ErrorAction SilentlyContinue

Write-Host "Build directories removed."
