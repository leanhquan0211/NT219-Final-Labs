# NT219 Final Cryptography Labs

This repository contains the implementation for Cryptography & Applications Laboratory Series, Labs 1-6.

## Project Structure

```text
Final_lab/
├── common/
├── lab1_aes_cryptopp/
├── lab2_aes_pure_cpp/
├── lab3_rsa_hybrid/
├── lab4_hash_pki_attacks/
├── lab5_digital_signatures/
├── lab6_pqc/
├── tests/
├── vectors/
├── samples/
├── scripts/
├── benchmarks/
├── report/
└── bin/
```

## Labs

| Lab | Topic | Tool |
|---|---|---|
| Lab 1 | Symmetric Encryption with Crypto++ | aestool |
| Lab 2 | AES-128 CTR in Pure C++ | aesctrtool |
| Lab 3 | RSA-OAEP and Hybrid Encryption | rsatool |
| Lab 4 | Hashing, PKI, and Practical Attacks | hashtool |
| Lab 5 | Classical Digital Signatures | sigtool |
| Lab 6 | Post-Quantum Signatures and Certificates | pqtool |

## Build Strategy

Development builds:

```text
Windows: MinGW64 + Ninja
Linux: WSL2 Ubuntu + Ninja
```

Final validation builds:

```text
Windows: MinGW64
Windows: MSVC
Linux: WSL2 Ubuntu
```

## Dependencies

- C++17 or later
- CMake
- Ninja
- Crypto++
- OpenSSL
- nlohmann-json
- Catch2
- Python 3 for benchmark plotting

## Build

Detailed build instructions are available in:

```text
docs/build_guide.md
```

## Academic Integrity

All implementations, tests, and reports are prepared for defensive education and academic learning only.

