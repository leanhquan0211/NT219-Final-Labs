# Lab 1 Stage 3B Notes — AES Crypto++

## Current implementation status

The `aestool` command now has a working Crypto++ AES foundation. It supports:

- ECB
- CBC
- CFB
- OFB
- CTR
- GCM
- CCM

AES-XTS is recognized by the mode parser but is intentionally marked as reserved for a later patch. This is tracked as a current limitation.

## Implemented commands

```text
aestool keygen
aestool encrypt
aestool decrypt
aestool kat
aestool bench
aestool selftest
aestool version
```

## KAT coverage added in Stage 3B

Stage 3B adds a larger KAT file:

```text
vectors/lab1/aes_stage3b_kat_vectors.json
```

It includes:

- AES-128-CBC from NIST SP 800-38A
- AES-128-CFB from NIST SP 800-38A
- AES-128-OFB from NIST SP 800-38A
- AES-128-CTR from NIST SP 800-38A
- AES-128-GCM empty plaintext smoke vector
- AES-128-GCM one-block smoke vector

## Negative tests added in Stage 3B

Automated unit tests now cover:

- GCM tampered ciphertext rejection
- Invalid AES key length
- Invalid IV length
- AAD rejected for non-AEAD modes
- ECB large input blocked without `--allow-ecb`
- AES-XTS currently reported as reserved

A Windows script is also provided:

```text
scripts/windows/lab1_run_negative_tests.ps1
```

## Benchmark script

Windows:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\windows\lab1_run_benchmarks.ps1 -ProjectRoot . -Configuration mingw -Runs 30
```

Linux:

```bash
./scripts/linux/lab1_run_benchmarks.sh /mnt/n/UIT/HK2/NT219/Final_lab linux 30
```

The benchmark tests:

- CBC
- CTR
- GCM
- CCM

Payload sizes:

- 1 KiB
- 4 KiB
- 16 KiB
- 256 KiB
- 1 MiB
- 8 MiB

Output is written to:

```text
benchmarks/results/
```

These CSV files are intentionally ignored by Git and should be copied into the final submission archive/report artifacts when needed.

## Security notes for report

### ECB

ECB is insecure because equal plaintext blocks produce equal ciphertext blocks under the same key. `aestool` prints a warning when ECB is selected and blocks inputs larger than 16 KiB unless the user explicitly passes `--allow-ecb`.

### CTR

CTR mode turns AES into a stream cipher. Reusing the same key and counter/nonce repeats the keystream, which can reveal relationships between plaintexts. Current implementation validates IV size but does not yet persist a nonce-reuse database.

### GCM/CCM

GCM and CCM provide authenticated encryption. The current implementation appends the authentication tag to the ciphertext. Tampering with ciphertext or tag causes decryption to fail closed.

### Current limitations

- AES-XTS is not implemented yet.
- Persistent nonce-reuse tracking is not implemented yet.
- Full official CCM test corpus is not included yet.
- Batch plotting is not implemented yet.
