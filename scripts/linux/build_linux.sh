#!/usr/bin/env bash
set -e

cd /mnt/n/UIT/HK2/NT219/Final_lab

cmake -B build-linux -S . \
  -G Ninja \
  -DCMAKE_BUILD_TYPE=Release

cmake --build build-linux

echo "Linux build completed."
