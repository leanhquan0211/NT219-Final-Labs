#!/usr/bin/env bash
set -e

sudo apt update
sudo apt install -y build-essential cmake ninja-build git pkg-config
sudo apt install -y libssl-dev libcrypto++-dev libcrypto++-utils
sudo apt install -y nlohmann-json3-dev catch2
sudo apt install -y python3 python3-pip
sudo apt install -y python3-numpy python3-pandas python3-matplotlib python3-scipy

echo "Ubuntu setup completed."
g++ --version
cmake --version
ninja --version
openssl version -a
