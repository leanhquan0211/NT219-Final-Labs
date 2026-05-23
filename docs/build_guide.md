# Build Guide

## Windows MinGW64 Build

Run in Windows PowerShell:

```powershell
cd N:\UIT\HK2\NT219\Final_lab

cmake -B build-mingw -S . `
  -G "Ninja" `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake `
  -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic

cmake --build build-mingw
```

## Windows MSVC Build

Run in Developer PowerShell for Visual Studio 2026:

```powershell
cd N:\UIT\HK2\NT219\Final_lab

cmake -B build-msvc -S . `
  -G "Ninja" `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake `
  -DVCPKG_TARGET_TRIPLET=x64-windows

cmake --build build-msvc
```

## Linux WSL2 Ubuntu Build

Run in WSL2 Ubuntu:

```bash
cd /mnt/n/UIT/HK2/NT219/Final_lab

cmake -B build-linux -S . \
  -G Ninja \
  -DCMAKE_BUILD_TYPE=Release

cmake --build build-linux
```

## Run Tests

Windows:

```powershell
ctest --test-dir build-mingw --output-on-failure
```

Linux:

```bash
ctest --test-dir build-linux --output-on-failure
```
