# Lab 1 GUI Usage

## Run GUI

```powershell
python gui\lab1_python\lab1_gui.py
```

---

## Supported Backends

* mingw
* msvc
* linux

The GUI automatically searches for exported shared libraries from:

* build-mingw
* build-msvc
* build-linux
* gui/lab1_python/native

---

## Example CTR Test

Backend:

```text
mingw
```

Mode:

```text
ctr
```

Key:

```text
00112233445566778899aabbccddeeff
```

IV:

```text
0102030405060708090a0b0c0d0e0f10
```

Plaintext:

```text
Hello NT219 Stage 3G GUI Test
```

---

## Example GCM Test

Mode:

```text
gcm
```

Nonce:

```text
0102030405060708090a0b0c
```

AAD:

```text
lab1 aad test
```

---

## Fail-Closed Verification

Modify one byte in the ciphertext/tag and attempt decryption.

Expected result:

```text
HashVerificationFilter: message hash or MAC not valid
```

This demonstrates authenticated encryption integrity verification.
