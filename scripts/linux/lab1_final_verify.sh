#!/usr/bin/env bash
set -euo pipefail

PROJECT_ROOT="${1:-/mnt/n/UIT/HK2/NT219/Final_lab}"
CONFIGURATION="${2:-linux}"
BENCHMARK_RUNS="${3:-5}"

cd "$PROJECT_ROOT"

AES_TOOL="$PROJECT_ROOT/build-linux/bin/aestool"
if [ ! -x "$AES_TOOL" ]; then
  echo "aestool not found: $AES_TOOL" >&2
  exit 1
fi

mkdir -p "$PROJECT_ROOT/report/lab1"
LOG_FILE="$PROJECT_ROOT/report/lab1/lab1_final_verification_log_linux.md"

{
  echo "# Lab 1 Final Verification Log — Linux"
  echo
  echo "Generated at: $(date '+%Y-%m-%d %H:%M:%S')"
  echo
  echo "Configuration: $CONFIGURATION"
  echo
} > "$LOG_FILE"

run_ok() {
  echo
  echo ">>> $*"
  "$@"
}

run_fail() {
  echo
  echo ">>> $*"
  set +e
  "$@"
  local code=$?
  set -e
  if [ "$code" -eq 0 ]; then
    echo "Expected command to fail, but it succeeded." >&2
    exit 1
  fi
  echo "Expected failure observed. Exit code: $code"
}

run_ok "$AES_TOOL" selftest
run_ok "$AES_TOOL" kat --kat vectors/lab1/aes_stage3b_kat_vectors.json

mkdir -p "$PROJECT_ROOT/samples/keys"

KEY_FILE="$PROJECT_ROOT/samples/keys/lab1_final_aes256_linux.key"
GCM_CT="$PROJECT_ROOT/samples/lab1_final_gcm_linux.ct"
GCM_META="$PROJECT_ROOT/samples/lab1_final_gcm_linux.json"
GCM_OUT="$PROJECT_ROOT/samples/lab1_final_gcm_recovered_linux.txt"

run_ok "$AES_TOOL" keygen --size 32 --out "$KEY_FILE"
run_ok "$AES_TOOL" encrypt --mode gcm --key "$KEY_FILE" --text "Lab1 final verification" --aad-text "stage3d" --out "$GCM_CT" --meta "$GCM_META"
run_ok "$AES_TOOL" decrypt --mode gcm --key "$KEY_FILE" --in "$GCM_CT" --aad-text "stage3d" --meta "$GCM_META" --out "$GCM_OUT"

if [ "$(cat "$GCM_OUT")" != "Lab1 final verification" ]; then
  echo "GCM recovered text mismatch." >&2
  exit 1
fi

TAMPERED_CT="$PROJECT_ROOT/samples/lab1_final_gcm_tampered_linux.ct"
cp "$GCM_CT" "$TAMPERED_CT"
python3 - "$TAMPERED_CT" <<'PY'
from pathlib import Path
import sys
p = Path(sys.argv[1])
data = bytearray(p.read_bytes())
data[0] ^= 1
p.write_bytes(data)
PY

run_fail "$AES_TOOL" decrypt --mode gcm --key "$KEY_FILE" --in "$TAMPERED_CT" --aad-text "stage3d" --meta "$GCM_META" --out "$PROJECT_ROOT/samples/lab1_final_tampered_out_linux.txt"

{
  echo "## Final checks"
  echo "- selftest: PASS"
  echo "- KAT smoke set: PASS"
  echo "- GCM round-trip: PASS"
  echo "- GCM tamper rejection: PASS"
  echo
  echo "## Known limitations still tracked"
  echo "- XTS mode is not yet implemented in the current foundation."
  echo "- Full no-padding NIST CBC/CFB KAT path is tracked for a later hardening patch."
  echo "- CTR/GCM/CCM nonce reuse persistence database is tracked for a later hardening patch."
} >> "$LOG_FILE"

echo
echo "Lab 1 Linux final verification log written to:"
echo "$LOG_FILE"
