# Lab 1 Completion Status

## Implemented features

| Requirement | Status |
|---|---:|
| Crypto++ AES implementation | Done |
| AES-ECB | Done, warning + size restriction |
| AES-CBC | Done |
| AES-CFB | Done |
| AES-OFB | Done |
| AES-CTR | Done |
| AES-GCM | Done |
| AES-CCM | Done |
| AES-XTS | Done |
| Key loading from file / hex | Done |
| IV/nonce loading from file / hex | Done |
| Secure IV/nonce generation | Done |
| Sidecar metadata JSON | Done |
| AEAD AAD support | Done |
| AEAD tag verification | Done |
| KAT runner | Done |
| Negative tests | Done |
| Benchmark scripts | Done |
| Plot/report support | Done |
| Optional nonce reuse tracking | Done via `--nonce-db` |

## Security notes

AES-GCM and AES-CCM provide authenticated encryption. Tampering with ciphertext or tag causes decryption failure. AES-CTR, AES-CFB, AES-OFB and AES-XTS do not provide integrity and must be paired with authentication in real deployments. AES-ECB is intentionally discouraged by warning and by blocking large inputs unless the user explicitly passes `--allow-ecb`.

## Limitations

The nonce reuse database is opt-in to avoid forcing global state during automated tests and benchmarking. The KAT collection is representative and validates the tool path, including no-padding CBC, CTR, OFB, and GCM vectors, but it is not a complete import of every official NIST vector file.

## Decision

Lab 1 is closed for now and the project can proceed to Lab 2. Further hardening may be done later if time remains.
