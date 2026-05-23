#!/usr/bin/env bash
set -euo pipefail

PROJECT_ROOT="${1:-$(pwd)}"
CONFIGURATION="${2:-linux}"
RUNS="${3:-30}"

cd "$PROJECT_ROOT"

if [[ "$CONFIGURATION" == "linux" ]]; then
  AES_TOOL="$PROJECT_ROOT/build-linux/bin/aestool"
else
  AES_TOOL="$PROJECT_ROOT/build-mingw/bin/aestool.exe"
fi

if [[ ! -x "$AES_TOOL" ]]; then
  echo "Cannot find executable aestool at $AES_TOOL" >&2
  exit 1
fi

RESULTS_DIR="$PROJECT_ROOT/benchmarks/results"
mkdir -p "$RESULTS_DIR"
mkdir -p "$PROJECT_ROOT/samples/keys"

KEY_PATH="$PROJECT_ROOT/samples/keys/lab1_bench_aes256.key"
"$AES_TOOL" keygen --size 32 --out "$KEY_PATH"

TIMESTAMP="$(date +%Y%m%d_%H%M%S)"
OUT_CSV="$RESULTS_DIR/lab1_${CONFIGURATION}_${TIMESTAMP}.csv"

echo "mode,size_bytes,runs,mean_ms,median_ms,stddev_ms,ci95_low_ms,ci95_high_ms,throughput_mib_s" > "$OUT_CSV"

MODES=(cbc ctr gcm ccm)
SIZES=(1024 4096 16384 262144 1048576 8388608)

for mode in "${MODES[@]}"; do
  for size in "${SIZES[@]}"; do
    echo "Benchmarking mode=$mode size=$size runs=$RUNS"
    output="$("$AES_TOOL" bench --mode "$mode" --key "$KEY_PATH" --size "$size" --runs "$RUNS")"
    echo "$output" | tail -n 1 >> "$OUT_CSV"
  done
done

echo "Benchmark CSV written to:"
echo "$OUT_CSV"
