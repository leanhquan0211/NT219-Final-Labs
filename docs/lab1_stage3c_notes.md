# Stage 3C Notes — Lab 1 plots and report support

Stage 3C adds support files for turning Lab 1 benchmark CSV files into report-ready artifacts.

## Added files

```text
scripts/python/lab1_plot_benchmarks.py
scripts/windows/lab1_generate_plots.ps1
scripts/windows/lab1_cleanup_generated.ps1
scripts/linux/lab1_generate_plots.sh
docs/lab1_completion_checklist.md
report/lab1/lab1_report_section_draft.md
```

## Plot generation

Windows:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\windows\lab1_generate_plots.ps1 -ProjectRoot N:\UIT\HK2\NT219\Final_lab
```

Linux:

```bash
./scripts/linux/lab1_generate_plots.sh /mnt/n/UIT/HK2/NT219/Final_lab python3
```

## Output

```text
benchmarks/plots/lab1_benchmark_summary.csv
benchmarks/plots/lab1_aes_throughput.png
benchmarks/plots/lab1_aes_latency.png
benchmarks/plots/lab1_aes_latency_ci_largest_payload.png
```

These generated files are report artifacts. They are ignored by Git by default unless explicitly added later for the final submission archive.
