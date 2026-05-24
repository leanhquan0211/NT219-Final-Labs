# Lab 1 Evidence Summary

## Build and Test Evidence

| Environment | Build log | CTest log | Selftest | Final KAT |
|---|---|---|---|---|
| Windows MinGW64 | final_logs/windows_mingw_build.txt | final_logs/windows_mingw_ctest.txt | final_logs/windows_mingw_selftest.txt | final_logs/windows_mingw_final_kat.txt |
| Windows MSVC | final_logs/windows_msvc_build.txt | final_logs/windows_msvc_ctest.txt | final_logs/windows_msvc_selftest.txt | final_logs/windows_msvc_final_kat.txt |
| Linux/WSL | final_logs/linux_build.txt | final_logs/linux_ctest.txt | final_logs/linux_selftest.txt | final_logs/linux_final_kat.txt |

## Benchmark Coverage

The benchmark scripts in Stage 3F v6 cover all eight Lab 1 AES modes:

- ECB
- CBC
- CFB
- OFB
- CTR
- XTS
- CCM
- GCM

Payload sizes:

- 1 KiB
- 4 KiB
- 16 KiB
- 256 KiB
- 1 MiB
- 8 MiB

Benchmark CSV files are stored in:

    report/lab1/benchmark_results/
    report/tables/lab1/

Benchmark plots are stored in:

    report/lab1/benchmark_plots/
    report/figures/lab1/

## Binary Deliverables

Final binaries are copied to:

    bin/windows-mingw/
    bin/windows-msvc/
    bin/linux/
