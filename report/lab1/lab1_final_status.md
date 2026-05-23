# Lab 1 Final Status — Symmetric Encryption with Crypto++

## Implementation Summary

The Lab 1 tool is `aestool`. It is implemented in modern C++ and uses Crypto++ for symmetric encryption primitives.

Implemented commands:

| Command | Purpose |
|---|---|
| `keygen` | Generate AES key material |
| `encrypt` | Encrypt file or text input |
| `decrypt` | Decrypt ciphertext input |
| `kat` | Run JSON-based known-answer tests |
| `bench` | Run benchmark measurements |
| `selftest` | Run a local smoke self-test |
| `version` | Print tool version |

Implemented AES modes:

| Mode | Status | Notes |
|---|---:|---|
| ECB | Implemented | Warns about insecurity; blocks large input unless `--allow-ecb` is used |
| CBC | Implemented | Uses padding in normal tool operation |
| CFB | Implemented | Round-trip tested |
| OFB | Implemented | KAT smoke tested |
| CTR | Implemented | KAT smoke tested |
| GCM | Implemented | AEAD tag verification and tamper rejection tested |
| CCM | Implemented | Available in foundation implementation |
| XTS | Not yet implemented | Tracked limitation |

## Security Controls Implemented

- Secure random generation through Crypto++ / common random helpers.
- IV/nonce auto-generation for encryption when omitted.
- JSON metadata sidecar for generated IV/nonce and mode metadata.
- AEAD tag verification for GCM/CCM.
- Fail-closed behavior for GCM tampered ciphertext.
- ECB warning.
- ECB input-size restriction unless `--allow-ecb` is explicitly provided.

## Validation Status

Current validation includes:

- Cross-platform build checks on Windows MinGW, Windows MSVC, and WSL2 Ubuntu.
- Catch2 unit tests.
- KAT smoke tests for CTR and OFB.
- Negative tests for GCM tampering and ECB misuse.
- Benchmark smoke tests for selected modes and payload sizes.
- Plot generation support for benchmark CSV files.

## Known Limitations

The current Lab 1 implementation is strong enough as a working foundation, but the following limitations should remain visible in the report:

1. AES-XTS is not implemented yet.
2. The current JSON KAT runner uses smoke vectors, not a complete official NIST corpus.
3. CBC no-padding NIST vectors are not directly compared through the normal encryption path because the CLI encryption path uses padding.
4. Persistent nonce-reuse detection for CTR/GCM/CCM is not implemented yet.
5. Full CCM official-vector coverage requires additional tag/AAD vector handling.

## Recommended Stage 3E Decision

Before closing Lab 1 completely, decide one of two paths:

### Path A — Complete Feature Closure

Implement:

- AES-XTS
- Dedicated no-padding KAT path
- GCM/CCM official KAT parsing
- Persistent nonce-reuse tracking

### Path B — Practical Submission Closure

Keep the current implementation and document the remaining limitations honestly in the report. This is acceptable only if time is limited and the report clearly states what is implemented and what is not.
