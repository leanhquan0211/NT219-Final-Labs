# Lab 1 Stage 3G — Library Export, GUI, and Environment Comparison

## Library Export

The Lab 1 AES core was exported through a C-compatible API. The exported API is declared in `lab1_aes_cryptopp/include/lab1/lab1_c_api.h` and implemented in `lab1_aes_cryptopp/src/lab1_c_api.cpp`.

The shared library target is `nt219_lab1_aes_shared`. It builds to a Windows DLL or Linux shared object depending on the platform. Static libraries are also preserved as `.a` or `.lib` artifacts.

## GUI Design

A minimal Python GUI was implemented in `gui/lab1_python/lab1_gui.py`. The GUI calls the compiled shared library through `ctypes` and does not duplicate AES logic. It supports selecting AES mode, entering hex-encoded key/IV/AAD/input, and running encryption or decryption.

## Benchmark Comparison Across Environments

Stage 3G also adds environment-aware benchmark summarization. The generated tables and plots compare MinGW, MSVC, and Linux/WSL results. The comparison is intended for the final report discussion on cross-platform performance.

Generated files:

- `report/tables/lab1/lab1_benchmark_environment_summary.csv`
- `report/tables/lab1/lab1_benchmark_environment_summary.md`
- `report/figures/lab1/lab1_aes_throughput_by_environment.png`
- `report/figures/lab1/lab1_aes_latency_by_environment.png`

## Limitations

The GUI is intentionally minimal. It focuses on demonstrating that the compiled library can be reused by an external application. It uses hex input/output to keep the interface deterministic and binary-safe.
