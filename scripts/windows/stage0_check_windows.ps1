Set-Location "N:\UIT\HK2\NT219\Final_lab"

Write-Host "=== Project tree ==="
tree /A

Write-Host "=== Tool versions ==="
code --version
cmake --version
git --version
g++ --version
ninja --version

Write-Host "=== vcpkg packages ==="
if (Test-Path "C:\vcpkg\vcpkg.exe") {
    C:\vcpkg\vcpkg.exe list
} else {
    Write-Host "C:\vcpkg\vcpkg.exe not found"
}
