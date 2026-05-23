#!/usr/bin/env bash
set -euo pipefail

PROJECT_ROOT="${1:-/mnt/n/UIT/HK2/NT219/Final_lab}"
CONFIGURATION="${2:-linux}"
cd "$PROJECT_ROOT"

if [[ "$CONFIGURATION" == "linux" ]]; then
  AES_TOOL="$PROJECT_ROOT/build-linux/bin/aestool"
else
  AES_TOOL="$PROJECT_ROOT/build-mingw/bin/aestool.exe"
fi

SAMPLE_DIR="$PROJECT_ROOT/samples"
KEY_DIR="$PROJECT_ROOT/samples/keys"
mkdir -p "$SAMPLE_DIR" "$KEY_DIR"

echo "Running Lab 1 completion tests with $AES_TOOL"

XTS_KEY="$KEY_DIR/lab1_completion_xts.key"
XTS_PLAIN="$SAMPLE_DIR/lab1_completion_xts_plain.bin"
XTS_CT="$SAMPLE_DIR/lab1_completion_xts.ct"
XTS_RECOVERED="$SAMPLE_DIR/lab1_completion_xts_recovered.bin"
XTS_META="$SAMPLE_DIR/lab1_completion_xts.json"

"$AES_TOOL" keygen --size 32 --out "$XTS_KEY"
python3 - <<PY
from pathlib import Path
Path(r"$XTS_PLAIN").write_bytes(bytes(range(64)))
PY
"$AES_TOOL" encrypt --mode xts --key "$XTS_KEY" --iv-hex 000102030405060708090a0b0c0d0e0f --in "$XTS_PLAIN" --out "$XTS_CT" --meta "$XTS_META"
"$AES_TOOL" decrypt --mode xts --key "$XTS_KEY" --meta "$XTS_META" --in "$XTS_CT" --out "$XTS_RECOVERED"
cmp "$XTS_PLAIN" "$XTS_RECOVERED"
echo "PASS: AES-XTS round trip succeeded."

GCM_KEY="$KEY_DIR/lab1_completion_gcm.key"
NONCE_DB="$SAMPLE_DIR/lab1_completion_nonce_db.json"
GCM_CT1="$SAMPLE_DIR/lab1_completion_gcm_1.ct"
GCM_CT2="$SAMPLE_DIR/lab1_completion_gcm_2.ct"
rm -f "$NONCE_DB"
"$AES_TOOL" keygen --size 32 --out "$GCM_KEY"
"$AES_TOOL" encrypt --mode gcm --key "$GCM_KEY" --nonce-hex 000102030405060708090a0b --text "nonce test" --out "$GCM_CT1" --nonce-db "$NONCE_DB"
if "$AES_TOOL" encrypt --mode gcm --key "$GCM_KEY" --nonce-hex 000102030405060708090a0b --text "nonce test again" --out "$GCM_CT2" --nonce-db "$NONCE_DB"; then
  echo "ERROR: Nonce reuse was not rejected" >&2
  exit 1
fi
echo "PASS: AES-GCM repeated key+nonce was rejected by nonce DB."

"$AES_TOOL" kat --kat vectors/lab1/aes_stage3e_final_kat_vectors.json

echo "All Lab 1 completion tests passed."
