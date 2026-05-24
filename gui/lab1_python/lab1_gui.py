import ctypes
import os
import platform
import sys
from pathlib import Path

from PySide6.QtWidgets import (
    QApplication,
    QWidget,
    QVBoxLayout,
    QHBoxLayout,
    QLabel,
    QComboBox,
    QTextEdit,
    QLineEdit,
    QPushButton,
    QMessageBox,
    QFileDialog,
)

ROOT = Path(__file__).resolve().parents[2]
GUI_DIR = Path(__file__).resolve().parent
NATIVE_DIR = GUI_DIR / "native"

MODES = ["ecb", "cbc", "cfb", "ofb", "ctr", "xts", "ccm", "gcm"]
BACKENDS = ["mingw", "msvc", "linux", "browse"]

_DLL_DIR_HANDLES = []


def is_windows() -> bool:
    return platform.system().lower().startswith("win")


def is_linux() -> bool:
    return platform.system().lower() == "linux"


def candidates_for_backend(backend: str):
    paths = []
    if backend == "mingw":
        paths += [
            NATIVE_DIR / "mingw" / "libnt219_lab1_aes_shared.dll",
            ROOT / "build-mingw" / "bin" / "libnt219_lab1_aes_shared.dll",
            ROOT / "bin" / "windows-mingw" / "libnt219_lab1_aes_shared.dll",
        ]
    elif backend == "msvc":
        paths += [
            NATIVE_DIR / "msvc" / "nt219_lab1_aes_shared.dll",
            NATIVE_DIR / "msvc" / "libnt219_lab1_aes_shared.dll",
            ROOT / "build-msvc" / "bin" / "nt219_lab1_aes_shared.dll",
            ROOT / "build-msvc" / "bin" / "libnt219_lab1_aes_shared.dll",
            ROOT / "bin" / "windows-msvc" / "nt219_lab1_aes_shared.dll",
            ROOT / "bin" / "windows-msvc" / "libnt219_lab1_aes_shared.dll",
        ]
    elif backend == "linux":
        paths += [
            NATIVE_DIR / "linux" / "libnt219_lab1_aes_shared.so",
            ROOT / "build-linux" / "lab1_aes_cryptopp" / "libnt219_lab1_aes_shared.so",
            ROOT / "build-linux" / "bin" / "libnt219_lab1_aes_shared.so",
            ROOT / "bin" / "linux" / "libnt219_lab1_aes_shared.so",
        ]
    return paths


def find_library(backend: str) -> Path:
    if backend == "browse":
        raise RuntimeError("Browse backend requires manual file selection.")
    for p in candidates_for_backend(backend):
        if p.exists():
            return p
    checked = "\n".join(str(p) for p in candidates_for_backend(backend))
    raise FileNotFoundError(f"Could not find shared library for backend '{backend}'. Checked:\n{checked}")


def _prepend_path(path: Path):
    path_str = str(path)
    old = os.environ.get("PATH", "")
    if path.exists() and path_str.lower() not in old.lower().split(os.pathsep):
        os.environ["PATH"] = path_str + os.pathsep + old


def add_library_search_paths(lib_path: Path):
    dirs = [
        lib_path.parent,
        NATIVE_DIR / "mingw",
        NATIVE_DIR / "msvc",
        ROOT / "build-mingw" / "bin",
        ROOT / "build-msvc" / "bin",
        ROOT / "bin" / "windows-mingw",
        ROOT / "bin" / "windows-msvc",
    ]

    if is_windows():
        for d in dirs:
            if d.exists():
                _prepend_path(d)
                if hasattr(os, "add_dll_directory"):
                    try:
                        _DLL_DIR_HANDLES.append(os.add_dll_directory(str(d)))
                    except OSError:
                        pass


class Lab1Library:
    def __init__(self, path: Path):
        self.path = path
        add_library_search_paths(path)
        self.lib = ctypes.CDLL(str(path))
        self._bind()

    def _bind(self):
        for name in ["nt219_lab1_encrypt_hex", "nt219_lab1_decrypt_hex"]:
            try:
                fn = getattr(self.lib, name)
            except AttributeError as exc:
                raise AttributeError(
                    f"Missing C API symbol '{name}' in {self.path}. Rebuild Stage 3G shared library."
                ) from exc
            fn.argtypes = [
                ctypes.c_char_p,
                ctypes.c_char_p,
                ctypes.c_char_p,
                ctypes.c_char_p,
                ctypes.c_char_p,
                ctypes.c_char_p,
                ctypes.c_size_t,
                ctypes.c_char_p,
                ctypes.c_size_t,
            ]
            fn.restype = ctypes.c_int

    def _call(self, fn_name: str, mode: str, key_hex: str, iv_hex: str, data: str, aad: str) -> str:
        out = ctypes.create_string_buffer(1024 * 1024 * 16)
        err = ctypes.create_string_buffer(8192)
        rc = getattr(self.lib, fn_name)(
            mode.encode("utf-8"),
            key_hex.encode("utf-8"),
            iv_hex.encode("utf-8"),
            data.encode("utf-8"),
            aad.encode("utf-8"),
            out,
            ctypes.sizeof(out),
            err,
            ctypes.sizeof(err),
        )
        if rc != 0:
            msg = err.value.decode("utf-8", errors="replace") or f"{fn_name} failed with code {rc}"
            raise RuntimeError(msg)
        return out.value.decode("utf-8", errors="replace")

    def encrypt(self, mode, key_hex, iv_hex, plaintext, aad):
        return self._call("nt219_lab1_encrypt_hex", mode, key_hex, iv_hex, plaintext, aad)

    def decrypt(self, mode, key_hex, iv_hex, ciphertext_hex, aad):
        return self._call("nt219_lab1_decrypt_hex", mode, key_hex, iv_hex, ciphertext_hex, aad)


class MainWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("NT219 Lab 1 AES GUI")
        self.resize(950, 700)
        self.lib = None
        self.lib_path = None
        self._build_ui()
        self._auto_select_backend()

    def _build_ui(self):
        layout = QVBoxLayout(self)

        row1 = QHBoxLayout()
        row1.addWidget(QLabel("Backend:"))
        self.backend = QComboBox()
        self.backend.addItems(BACKENDS)
        self.backend.currentTextChanged.connect(self.load_backend)
        row1.addWidget(self.backend)

        self.reload_btn = QPushButton("Reload backend")
        self.reload_btn.clicked.connect(lambda: self.load_backend(self.backend.currentText()))
        row1.addWidget(self.reload_btn)

        self.browse_btn = QPushButton("Browse library")
        self.browse_btn.clicked.connect(self.browse_library)
        row1.addWidget(self.browse_btn)

        row1.addWidget(QLabel("Mode:"))
        self.mode = QComboBox()
        self.mode.addItems(MODES)
        row1.addWidget(self.mode)
        layout.addLayout(row1)

        self.path_label = QLabel("Library: not loaded")
        self.path_label.setWordWrap(True)
        layout.addWidget(self.path_label)

        layout.addWidget(QLabel("Key hex: 16/24/32 bytes. XTS requires double-length key, usually 64 bytes hex for AES-256-XTS."))
        self.key = QLineEdit("00112233445566778899aabbccddeeff")
        layout.addWidget(self.key)

        layout.addWidget(QLabel("IV / Nonce hex: ECB may leave empty; GCM normally 12 bytes; most other modes 16 bytes."))
        self.iv = QLineEdit("0102030405060708090a0b0c0d0e0f10")
        layout.addWidget(self.iv)

        layout.addWidget(QLabel("AAD text, used by GCM/CCM:"))
        self.aad = QLineEdit("")
        layout.addWidget(self.aad)

        layout.addWidget(QLabel("Input plaintext for Encrypt, or ciphertext hex for Decrypt:"))
        self.input = QTextEdit()
        self.input.setPlainText("Hello NT219 Stage 3G GUI Test")
        layout.addWidget(self.input)

        row2 = QHBoxLayout()
        self.encrypt_btn = QPushButton("Encrypt")
        self.encrypt_btn.clicked.connect(self.encrypt)
        row2.addWidget(self.encrypt_btn)
        self.decrypt_btn = QPushButton("Decrypt")
        self.decrypt_btn.clicked.connect(self.decrypt)
        row2.addWidget(self.decrypt_btn)
        layout.addLayout(row2)

        layout.addWidget(QLabel("Output:"))
        self.output = QTextEdit()
        layout.addWidget(self.output)

    def _auto_select_backend(self):
        preferred = "linux" if is_linux() else "mingw"
        index = self.backend.findText(preferred)
        if index >= 0:
            self.backend.setCurrentIndex(index)
        self.load_backend(self.backend.currentText())

    def browse_library(self):
        file_filter = "Shared libraries (*.dll *.so);;All files (*)"
        path, _ = QFileDialog.getOpenFileName(self, "Select Lab 1 shared library", str(ROOT), file_filter)
        if path:
            self.backend.setCurrentText("browse")
            self._load_path(Path(path))

    def load_backend(self, backend: str):
        if backend == "browse":
            self.path_label.setText("Library: browse mode; select a library file.")
            return
        try:
            if backend == "linux" and is_windows():
                self.path_label.setText("Library not loaded: Linux .so cannot be loaded by Windows Python. Use mingw/msvc on Windows.")
                self.lib = None
                return
            if backend in ["mingw", "msvc"] and not is_windows():
                self.path_label.setText("Library not loaded: Windows .dll cannot be loaded by Linux Python. Use linux on WSL/Linux.")
                self.lib = None
                return
            self._load_path(find_library(backend))
        except Exception as e:
            self.lib = None
            self.lib_path = None
            self.path_label.setText(f"Library not loaded: {e}")

    def _load_path(self, path: Path):
        try:
            self.lib = Lab1Library(path)
            self.lib_path = path
            self.path_label.setText(f"Library loaded: {path}")
        except Exception as e:
            self.lib = None
            self.lib_path = None
            self.path_label.setText(f"Library load failed: {e}")

    def _ensure_lib(self):
        if self.lib is None:
            raise RuntimeError("No backend library loaded. Build Stage 3G and run gui/lab1_python/collect_native_libs.ps1.")

    def encrypt(self):
        try:
            self._ensure_lib()
            result = self.lib.encrypt(
                self.mode.currentText(),
                self.key.text().strip(),
                self.iv.text().strip(),
                self.input.toPlainText(),
                self.aad.text(),
            )
            self.output.setPlainText(result)
        except Exception as e:
            QMessageBox.critical(self, "Encrypt failed", str(e))

    def decrypt(self):
        try:
            self._ensure_lib()
            result = self.lib.decrypt(
                self.mode.currentText(),
                self.key.text().strip(),
                self.iv.text().strip(),
                self.input.toPlainText().strip(),
                self.aad.text(),
            )
            self.output.setPlainText(result)
        except Exception as e:
            QMessageBox.critical(self, "Decrypt failed", str(e))


if __name__ == "__main__":
    app = QApplication(sys.argv)
    w = MainWindow()
    w.show()
    sys.exit(app.exec())
