#!/usr/bin/env bash
set -euo pipefail

PROJECT_ROOT="${1:-$(pwd)}"
cd "$PROJECT_ROOT"

mkdir -p report/figures/lab1 report/tables/lab1 report/lab1/benchmark_plots

python3 scripts/python/lab1_generate_environment_plots.py \
  --input-dir report/lab1/benchmark_results \
  --output-dir report/figures/lab1 \
  --tables-dir report/tables/lab1

cp report/figures/lab1/lab1_aes_*_by_environment.png report/lab1/benchmark_plots/ 2>/dev/null || true

echo "Stage 3G environment plots and summary tables generated."
