# Lab 1 — Symmetric Encryption with Crypto++

## 1. Objective

The objective of Lab 1 is to build a command-line AES encryption tool named `aestool` using the Crypto++ library. The tool is designed to support common AES modes, binary-safe file processing, text input, encoded output, metadata sidecar files, authenticated encryption modes, negative testing, and performance benchmarking.

The implementation follows the shared engineering standard of the project: all tools use C++17, CMake, out-of-source builds, common utility modules, Catch2 tests, and cross-platform validation on Windows and Linux.

## 2. Implemented functionality

The current implementation provides the following commands:

```text
aestool keygen
aestool encrypt
aestool decrypt
aestool kat
aestool bench
aestool selftest
aestool version
```

The implemented AES modes are:

```text
ECB, CBC, CFB, OFB, CTR, GCM, CCM
```

The command-line interface supports file and text input through `--in` and `--text`, file output through `--out`, key input through `--key` or `--key-hex`, IV/nonce input through `--iv`, `--iv-hex`, `--nonce`, or `--nonce-hex`, and encoded output through `--encode raw|hex|base64`.

## 3. Design and security choices

Crypto++ is used for all AES cryptographic primitives in this lab. Random key and IV/nonce generation uses secure random generation through the common random helper layer. For encryption modes requiring an IV or nonce, the tool can generate the value automatically during encryption and store it in a JSON metadata sidecar file.

For AEAD modes such as GCM and CCM, the authentication tag is verified during decryption. If ciphertext or tag verification fails, the tool rejects the input and does not silently produce plaintext. This is an important fail-closed behavior required for authenticated encryption.

ECB mode is intentionally restricted. When ECB is selected, the tool prints a warning because ECB leaks plaintext patterns. Inputs larger than 16 KiB are blocked unless the user explicitly passes `--allow-ecb`. This demonstrates misuse prevention rather than simply exposing insecure functionality.

## 4. Correctness testing

The project includes unit tests for AES round-trip encryption/decryption and negative cases. The current external KAT file covers exact CTR and OFB cases. CBC exact NIST comparison requires a no-padding KAT-only path because the normal CLI encryption path uses padding, while NIST SP 800-38A CBC examples are no-padding vectors.

The test suite is executed through CTest and has been validated on:

```text
Windows MinGW
Windows MSVC
WSL2 Ubuntu
```

## 5. Negative testing

Negative tests demonstrate the following behavior:

```text
GCM tampered ciphertext is rejected.
ECB large input is blocked unless --allow-ecb is provided.
Invalid input causes the tool to fail closed.
```

The GCM tamper test modifies the ciphertext and confirms that decryption fails due to authentication verification. This shows that the AEAD path detects active modification rather than returning corrupted plaintext.

## 6. Performance methodology

The benchmark command measures latency and throughput for AES modes over multiple payload sizes. Batch benchmark scripts run the benchmark for:

```text
1 KiB
4 KiB
16 KiB
256 KiB
1 MiB
8 MiB
```

The benchmark output is written as CSV and includes:

```text
mean latency
median latency
standard deviation
95% confidence interval
throughput in MiB/s
```

A Python plotting script converts benchmark CSV files into throughput and latency plots. These plots are generated under `benchmarks/plots` and are intended to be included in the final report.

## 7. Security discussion

ECB is insecure because identical plaintext blocks encrypt to identical ciphertext blocks under the same key. This leaks structural information about the plaintext. The implementation therefore warns the user and blocks large ECB inputs by default.

CBC provides confidentiality but not integrity. If used without authentication, tampering may not be detected reliably. CBC can also be vulnerable to padding oracle attacks if decryption errors are exposed incorrectly in a protocol setting.

CTR turns AES into a stream cipher. Reusing the same key and nonce is catastrophic because it reuses the keystream. The current implementation supports secure nonce generation but does not yet persist a nonce reuse database. This is listed as a known limitation for future improvement.

GCM and CCM provide authenticated encryption. They protect both confidentiality and integrity when nonces are unique. The implementation verifies the authentication tag and fails closed when verification fails.

## 8. Known limitations

The current implementation does not yet implement AES-XTS. The current KAT runner has limited coverage and should be expanded with a KAT-only no-padding path and AEAD-specific vector fields. Persistent nonce reuse tracking is also not yet implemented.

## 9. Conclusion

Lab 1 currently provides a functional AES command-line tool with multiple Crypto++ modes, secure random generation, JSON metadata sidecar support, AEAD verification, misuse checks, unit tests, negative tests, and benchmark scripts. The remaining work is to expand KAT coverage, add XTS or document it as a limitation, and implement persistent nonce reuse tracking.
