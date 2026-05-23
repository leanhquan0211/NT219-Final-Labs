#!/usr/bin/env bash
set -e

cd /mnt/n/UIT/HK2/NT219/Final_lab

echo "=== Current path ==="
pwd

echo "=== Project files ==="
ls

echo "=== Package status ==="
dpkg -s libssl-dev libcrypto++-dev nlohmann-json3-dev catch2 | grep -E "Package|Status"

echo "=== Tool versions ==="
g++ --version
cmake --version
ninja --version
openssl version -a
