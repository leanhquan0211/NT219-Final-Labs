#!/usr/bin/env bash
set -e

cd /mnt/n/UIT/HK2/NT219/Final_lab

ctest --test-dir build-linux --output-on-failure

echo "Linux tests completed."
