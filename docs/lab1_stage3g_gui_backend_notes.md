# Lab 1 Stage 3G GUI Backend Notes

The Lab 1 GUI supports backend selection for exported native libraries:

- `mingw`: loads `libnt219_lab1_aes_shared.dll` from MinGW build output or `gui/lab1_python/native/mingw/`.
- `msvc`: loads `nt219_lab1_aes_shared.dll` or `libnt219_lab1_aes_shared.dll` from MSVC build output or `gui/lab1_python/native/msvc/`.
- `linux`: loads `libnt219_lab1_aes_shared.so` from Linux build output or `gui/lab1_python/native/linux/`.

Windows Python can only load Windows DLLs. Linux/WSL Python can only load Linux `.so` files.

Before demo, run:

    powershell -NoProfile -ExecutionPolicy Bypass -File gui\lab1_python\collect_native_libs.ps1 -ProjectRoot N:\UIT\HK2\NT219\Final_lab

Then run:

    python gui\lab1_python\lab1_gui.py
