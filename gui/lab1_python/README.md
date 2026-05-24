# Lab 1 Python GUI

This GUI calls the exported Lab 1 AES shared library through Python `ctypes`.

## Collect native libraries

From PowerShell:

    cd N:\UIT\HK2\NT219\Final_lab
    powershell -NoProfile -ExecutionPolicy Bypass -File gui\lab1_python\collect_native_libs.ps1 -ProjectRoot N:\UIT\HK2\NT219\Final_lab

The script copies libraries into:

    gui/lab1_python/native/mingw/
    gui/lab1_python/native/msvc/
    gui/lab1_python/native/linux/

## Run on Windows

    cd N:\UIT\HK2\NT219\Final_lab
    python -m pip install -r gui\lab1_python\requirements.txt
    python gui\lab1_python\lab1_gui.py

On Windows, choose `mingw` or `msvc`. Windows Python cannot load Linux `.so` files.

## Run on Linux/WSL with GUI support

    cd /mnt/n/UIT/HK2/NT219/Final_lab
    python3 -m pip install -r gui/lab1_python/requirements.txt
    python3 gui/lab1_python/lab1_gui.py

On Linux/WSL, choose `linux`.
