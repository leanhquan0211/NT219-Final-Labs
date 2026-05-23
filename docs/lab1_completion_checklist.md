# Lab 1 Completion Checklist — AES Crypto++

## Implementation status

| Requirement | Status | Evidence |
|---|---:|---|
| Crypto++ used for AES primitives | Done | `lab1_aes_cryptopp/src/aes_crypto.cpp` |
| CLI tool `aestool` exists | Done | `lab1_aes_cryptopp/src/main.cpp` |
| Commands: `keygen`, `encrypt`, `decrypt`, `kat`, `bench`, `selftest`, `version` | Done | `aestool --help` |
| Modes: ECB, CBC, CFB, OFB, CTR, GCM, CCM | Partial/Done | Implemented in AES wrapper; XTS not yet implemented |
| AEAD tag verification | Done | GCM tamper negative test |
| ECB warning and large-input block | Done | `lab1_run_negative_tests.ps1` |
| Metadata sidecar JSON | Done | `--meta` option |
| KAT runner | Partial | CTR/OFB exact KAT currently; full CBC/GCM/CCM KAT requires Stage 3D |
| Unit tests | Done | Catch2 Lab 1 tests |
| Benchmark command | Done | `aestool bench` |
| Batch benchmark scripts | Done | Windows and Linux benchmark scripts |
| Plot generation | Done | `scripts/python/lab1_plot_benchmarks.py` |
| Report notes | Done | `report/lab1/lab1_report_section_draft.md` |

## Known limitations to report honestly

1. AES-XTS is listed in the lab requirement but is not implemented in the current Stage 3C patch.
2. The current external KAT JSON is limited to exact CTR/OFB cases because CBC in the CLI path uses padding, while NIST SP 800-38A CBC vectors are no-padding vectors.
3. Full GCM/CCM KAT coverage needs an expanded KAT format that stores AAD and authentication tag separately.
4. Nonce reuse persistence is not yet implemented. The current implementation generates IV/nonce securely but does not keep a persistent key+nonce reuse database.
5. Benchmark CSV/PNG outputs are generated artifacts and are not committed by default because `.gitignore` excludes them.

## Recommended next patch

Stage 3D should add:

1. KAT-only no-padding path for CBC/CFB/OFB/CTR.
2. KAT format supporting AEAD fields: `aad`, `tag`, `ciphertext`.
3. Optional XTS support or an explicit limitation section with justification.
4. Nonce reuse tracking for CTR/GCM/CCM through a local JSON database.
