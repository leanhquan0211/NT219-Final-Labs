# Lab 1 — AES Cryptographic Toolkit

## 1. Overview

This project implements an AES cryptographic toolkit using Crypto++ and OpenSSL.
The toolkit supports multiple AES operation modes, cross-platform builds, automated tests, benchmark evaluation, shared library export, and a graphical user interface.

The implementation was developed and validated on:

* Windows (MinGW64)
* Windows (MSVC)
* Linux / WSL

---

## 2. Supported AES Modes

The toolkit supports all eight required AES modes:

* ECB
* CBC
* CFB
* OFB
* CTR
* XTS
* CCM
* GCM

Authenticated encryption modes (CCM/GCM) support integrity verification and fail-closed behavior.

---

## 3. Security Features

### 3.1 Key Validation

The implementation validates AES key lengths according to the selected mode.

### 3.2 Nonce / IV Validation

The toolkit validates nonce and IV sizes for each mode.

### 3.3 Fail-Closed AEAD Verification

AES-GCM and AES-CCM reject modified ciphertext/tag combinations.

### 3.4 Nonce Tracking

The project includes nonce tracking to reduce accidental nonce reuse during testing.

---

## 4. Cross-Platform Build

The toolkit was successfully built and tested on:

| Environment     | Status |
| --------------- | ------ |
| Windows MinGW64 | PASS   |
| Windows MSVC    | PASS   |
| Linux / WSL     | PASS   |

---

## 5. Benchmark Evaluation

Benchmark testing was performed for all eight AES modes.

Measured environments:

* MinGW64
* MSVC
* Linux / WSL

Measured payload sizes:

* 1 KiB
* 4 KiB
* 16 KiB
* 256 KiB
* 1 MiB
* 8 MiB

Benchmark outputs include:

* Throughput plots
* Latency plots
* CSV benchmark tables
* Environment comparison plots

---

## 6. Shared Library Export

The AES toolkit was exported as reusable shared libraries.

### Windows

* nt219_lab1_aes_shared.dll

### Linux

* libnt219_lab1_aes_shared.so

The project also exports static/import libraries for reuse in other applications.

---

## 7. Python GUI

A PySide6-based GUI was implemented.

The GUI supports:

* AES encrypt/decrypt
* Multiple AES modes
* MinGW backend
* MSVC backend
* Automatic backend loading
* GCM/CCM integrity verification testing

GUI screenshots are included in the report figures section.

---

## 8. Testing

The project includes:

* Unit tests
* Known-answer tests (KAT)
* Negative tests
* AEAD tamper tests
* Cross-platform validation
* Benchmark validation

---

## 9. Limitations

Current limitations include:

* Linux backend GUI testing was limited under WSL GUI constraints
* GUI currently targets demonstration/testing usage
* Benchmark timing may vary depending on hardware and virtualization overhead

---

## 10. Conclusion

The project successfully implements a reusable cross-platform AES toolkit with:

* Eight AES modes
* Automated testing
* Benchmarking
* Shared library export
* Python GUI integration
* Cross-platform compatibility
