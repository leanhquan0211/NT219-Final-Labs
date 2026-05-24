# Lab 1 Stage 3G — Library Export and GUI Notes

Stage 3G adds the optional/bonus deliverables required by the project interpretation:

1. Export Lab 1 AES core as a C-compatible shared library.
2. Keep the existing static library deliverables.
3. Add a Python GUI that calls the compiled shared library through `ctypes`.
4. Add benchmark summary tables and environment-comparison plots.

## Exported library

Public C API:

- `lab1_aes_cryptopp/include/lab1/lab1_c_api.h`

Implementation:

- `lab1_aes_cryptopp/src/lab1_c_api.cpp`

Build outputs:

- Windows: `nt219_lab1_aes_shared.dll`
- Linux: `libnt219_lab1_aes_shared.so`
- Static libraries remain available as `.a` and `.lib`.

## GUI

GUI path:

- `gui/lab1_python/lab1_gui.py`

The GUI uses PySide6 for the interface and Python `ctypes` to call the shared library. It does not duplicate cryptographic logic.

## Benchmark environment comparison

Script:

- `scripts/python/lab1_generate_environment_plots.py`

Generated outputs:

- `report/tables/lab1/lab1_benchmark_environment_summary.csv`
- `report/tables/lab1/lab1_benchmark_environment_summary.md`
- `report/figures/lab1/lab1_aes_throughput_by_environment.png`
- `report/figures/lab1/lab1_aes_latency_by_environment.png`

These files support the final report section comparing MinGW, MSVC, and Linux/WSL performance.
