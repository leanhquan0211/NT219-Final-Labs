# Development Environment

## Project Root

Windows:

```text
N:\UIT\HK2\NT219\Final_lab
```

WSL2 Ubuntu:

```text
/mnt/n/UIT/HK2/NT219/Final_lab
```

## Windows Environment

| Component | Path / Version |
|---|---|
| VS Code | D:\Scoop\apps\vscode\current\bin\code |
| CMake | D:\Scoop\shims\cmake.exe, version 4.3.1 |
| Git | D:\Scoop\apps\git\current\cmd\git.exe / C:\msys64\usr\bin\git.exe |
| MinGW64 g++ | C:\msys64\mingw64\bin\g++.exe, GCC 15.2.0 |
| MSVC | Visual Studio 2026 Community, MSVC 19.50 |
| Ninja | D:\Scoop\shims\ninja.exe, version 1.13.2 |

## WSL2 Ubuntu Environment

| Component | Path / Version |
|---|---|
| Ubuntu | Ubuntu 24.04.4 LTS |
| Kernel | WSL2 |
| g++ | /usr/bin/g++, GCC 13.3.0 |
| CMake | /usr/bin/cmake, version 3.28.3 |
| Ninja | /usr/bin/ninja, version 1.11.1 |
| OpenSSL | /usr/bin/openssl, OpenSSL 3.0.13 |
| Crypto++ headers | /usr/include/crypto++ |

## Build Strategy

During development:

```text
Windows: MinGW64 + Ninja
Linux: WSL2 Ubuntu + Ninja
```

Before submission:

```text
Windows: MinGW64 build
Windows: MSVC build
Linux: WSL2 Ubuntu build
```
