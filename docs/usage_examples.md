# Usage Examples

## Lab 1 - AES Crypto++

```powershell
aestool encrypt --mode gcm --key samples\keys\aes.key --in samples\plaintext\msg.txt --out samples\tampered\ct.bin
aestool decrypt --mode gcm --key samples\keys\aes.key --in samples\tampered\ct.bin --out samples\plaintext\decrypted.txt
```

## Lab 2 - AES-128 CTR Pure C++

```powershell
aesctrtool encrypt --key samples\keys\aes128.key --iv samples\keys\iv.bin --in samples\plaintext\msg.bin --out samples\tampered\ct.bin
aesctrtool decrypt --key samples\keys\aes128.key --iv samples\keys\iv.bin --in samples\tampered\ct.bin --out samples\plaintext\dec.bin
```

## Lab 3 - RSA-OAEP and Hybrid Encryption

```powershell
rsatool keygen --bits 3072 --pub samples\keys\rsa_pub.pem --priv samples\keys\rsa_priv.pem
rsatool encrypt --in samples\plaintext\msg.bin --pub samples\keys\rsa_pub.pem --out samples\tampered\rsa_ct.bin
rsatool decrypt --in samples\tampered\rsa_ct.bin --priv samples\keys\rsa_priv.pem --out samples\plaintext\rsa_dec.bin
```

## Lab 4 - Hashing and PKI

```powershell
hashtool --algo sha256 --in samples\plaintext\msg.bin
hashtool --algo shake256 --outlen 64 --in samples\plaintext\msg.bin
hashtool cert --in samples\certs\cert.pem
```

## Lab 5 - Digital Signatures

```powershell
sigtool keygen --algo ecdsa-p256 --pub samples\keys\ecdsa_pub.pem --priv samples\keys\ecdsa_priv.pem
sigtool sign --algo ecdsa-p256 --in samples\plaintext\msg.bin --out samples\tampered\sig.bin --hash sha256
sigtool verify --algo ecdsa-p256 --in samples\plaintext\msg.bin --sig samples\tampered\sig.bin --pub samples\keys\ecdsa_pub.pem
```

## Lab 6 - Post-Quantum Cryptography

```powershell
pqtool keygen --algo mldsa-44 --pub samples\keys\mldsa_pub.pem --priv samples\keys\mldsa_priv.pem
pqtool sign --algo mldsa-44 --in samples\plaintext\msg.bin --out samples\tampered\pq_sig.bin
pqtool verify --algo mldsa-44 --in samples\plaintext\msg.bin --sig samples\tampered\pq_sig.bin --pub samples\keys\mldsa_pub.pem
```
