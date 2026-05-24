#pragma once

#include <stddef.h>

#if defined(_WIN32) || defined(__CYGWIN__)
  #if defined(NT219_LAB1_AES_SHARED_EXPORTS)
    #define NT219_LAB1_API __declspec(dllexport)
  #else
    #define NT219_LAB1_API __declspec(dllimport)
  #endif
#else
  #define NT219_LAB1_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

NT219_LAB1_API int nt219_lab1_encrypt_hex(
    const char* mode,
    const char* key_hex,
    const char* iv_hex,
    const char* plaintext_utf8,
    const char* aad_utf8,
    char* output_hex,
    size_t output_hex_size,
    char* error_buffer,
    size_t error_buffer_size);

NT219_LAB1_API int nt219_lab1_decrypt_hex(
    const char* mode,
    const char* key_hex,
    const char* iv_hex,
    const char* ciphertext_hex,
    const char* aad_utf8,
    char* output_utf8,
    size_t output_utf8_size,
    char* error_buffer,
    size_t error_buffer_size);

#ifdef __cplusplus
}
#endif
