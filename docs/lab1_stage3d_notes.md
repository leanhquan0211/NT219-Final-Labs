# Lab 1 Stage 3D Notes — Final Verification Support

## Purpose

Stage 3D adds final verification scripts and a clearer closure plan for Lab 1. It does not rewrite the AES core. The goal is to make the current Lab 1 implementation reproducible, testable, and easier to cite in the final report.

## What Stage 3D Adds

- `scripts/windows/lab1_final_verify.ps1`
- `scripts/linux/lab1_final_verify.sh`
- `report/lab1/lab1_final_status.md`

The final verification scripts perform:

1. `aestool selftest`
2. KAT smoke test with `vectors/lab1/aes_stage3b_kat_vectors.json`
3. AES-256-GCM encrypt/decrypt round-trip
4. GCM tamper rejection check
5. ECB large-input restriction check
6. Generation of a local verification log under `report/lab1/`

## Current Lab 1 Status

Implemented:

- AES-ECB
- AES-CBC
- AES-CFB
- AES-OFB
- AES-CTR
- AES-GCM
- AES-CCM
- `keygen`
- `encrypt`
- `decrypt`
- `kat`
- `bench`
- `selftest`
- `version`
- JSON metadata sidecar for IV/nonce/tag-related metadata
- AEAD tamper rejection
- ECB warning and large-input blocking
- Windows/Linux benchmark scripts
- Plot generation scripts
- Report draft notes

Tracked limitations:

- AES-XTS is not yet implemented.
- Full no-padding NIST CBC/CFB KAT execution is not yet implemented as a dedicated KAT path.
- Persistent nonce-reuse tracking for CTR/GCM/CCM is not yet implemented.
- Current KAT file is a smoke KAT set, not a complete official corpus.

## Closure Plan

Stage 3D should be followed by a short Stage 3E final audit. Stage 3E should decide whether to:

1. implement AES-XTS and nonce-reuse tracking now, or
2. explicitly document them as limitations if time is constrained.

For the final submission, it is better to be honest and explicit than to silently claim unsupported features.
