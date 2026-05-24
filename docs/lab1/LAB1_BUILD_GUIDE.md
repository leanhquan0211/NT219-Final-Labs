# Lab 1 Build Guide

## Windows MinGW64

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\scripts\windows\build_mingw.ps1
```

---

## Windows MSVC

Use:

```text
x64 Native Tools Command Prompt for VS 2026
```

Then:

```cmd
powershell -NoProfile -ExecutionPolicy Bypass -File .\scripts\windows\build_msvc.ps1
```

---

## Linux / WSL

```bash
./scripts/linux/build_linux.sh
```

---

## Benchmark

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\scripts\windows\lab1_run_benchmarks.ps1
```
