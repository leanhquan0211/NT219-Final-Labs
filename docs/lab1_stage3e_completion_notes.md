# Lab 1 Stage 3E Completion Notes

This stage completes the practical Lab 1 implementation baseline.

## Added in Stage 3E

- AES-XTS encryption/decryption support in the Lab 1 Crypto++ core.
- `aestool` support for `--mode xts`.
- AES-XTS round-trip tests.
- No-padding support for KAT-only AES-CBC vectors.
- Final KAT file: `vectors/lab1/aes_stage3e_final_kat_vectors.json`.
- Optional nonce reuse tracking with `--nonce-db FILE` for CTR/GCM/CCM.
- Windows/Linux completion test scripts.

## Security behavior

- ECB remains restricted for inputs larger than 16 KiB unless `--allow-ecb` is used.
- GCM and CCM authenticate ciphertext and reject tampered inputs.
- CTR/GCM/CCM nonce reuse can be rejected when the caller provides `--nonce-db`.
- XTS is implemented for disk-sector style confidentiality and does not provide integrity.

## Remaining limitations to disclose in report

- The nonce reuse database is opt-in through `--nonce-db`; it is not a global mandatory database.
- The official KAT set is representative, not exhaustive across every NIST file and every AES key size.
- XTS is tested by round trip and functional behavior; it is not yet backed by a large official XTS vector corpus.
- The CLI stores IV/nonce in a sidecar JSON when requested with `--meta`.

## Completion status

Lab 1 is now complete enough to move to Lab 2 while keeping the remaining limitations documented.
