#!/usr/bin/env bash
set -euo pipefail

PROJECT_ROOT="${1:-/mnt/n/UIT/HK2/NT219/Final_lab}"
PYTHON_BIN="${2:-python3}"

cd "$PROJECT_ROOT"

INPUT_DIR="$PROJECT_ROOT/benchmarks/results"
OUTPUT_DIR="$PROJECT_ROOT/benchmarks/plots"
SCRIPT_PATH="$PROJECT_ROOT/scripts/python/lab1_plot_benchmarks.py"

mkdir -p "$OUTPUT_DIR"

"$PYTHON_BIN" "$SCRIPT_PATH" --input-dir "$INPUT_DIR" --output-dir "$OUTPUT_DIR"

echo "Lab 1 plot generation completed."
