#include "lab1/lab1_c_api.h"
#include "lab1/aes_crypto.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void write_c_string(char* dst, std::size_t dst_size, const std::string& value) {
    if (dst == nullptr || dst_size == 0) {
        return;
    }
    const std::size_t n = std::min(dst_size - 1, value.size());
    std::memcpy(dst, value.data(), n);
    dst[n] = '\0';
}

int hex_value(char c) {
    const unsigned char uc = static_cast<unsigned char>(c);
    if (uc >= '0' && uc <= '9') return uc - '0';
    if (uc >= 'a' && uc <= 'f') return uc - 'a' + 10;
    if (uc >= 'A' && uc <= 'F') return uc - 'A' + 10;
    return -1;
}

std::vector<unsigned char> hex_to_bytes(const char* text) {
    std::string s = text ? std::string(text) : std::string();
    s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    }), s.end());

    if (s.size() % 2 != 0) {
        throw std::runtime_error("hex string must contain an even number of characters");
    }

    std::vector<unsigned char> out;
    out.reserve(s.size() / 2);
    for (std::size_t i = 0; i < s.size(); i += 2) {
        const int hi = hex_value(s[i]);
        const int lo = hex_value(s[i + 1]);
        if (hi < 0 || lo < 0) {
            throw std::runtime_error("hex string contains invalid character");
        }
        out.push_back(static_cast<unsigned char>((hi << 4) | lo));
    }
    return out;
}

std::string bytes_to_hex(const std::vector<unsigned char>& data) {
    static constexpr char digits[] = "0123456789abcdef";
    std::string out;
    out.resize(data.size() * 2);
    for (std::size_t i = 0; i < data.size(); ++i) {
        out[2 * i] = digits[(data[i] >> 4) & 0x0F];
        out[2 * i + 1] = digits[data[i] & 0x0F];
    }
    return out;
}

std::vector<unsigned char> utf8_to_bytes(const char* text) {
    if (text == nullptr) {
        return {};
    }
    const std::string s(text);
    return std::vector<unsigned char>(s.begin(), s.end());
}

nt219::lab1::AesRequest make_request(
    const char* mode,
    const char* key_hex,
    const char* iv_hex,
    const char* aad_utf8) {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::parse_aes_mode(mode ? std::string(mode) : std::string());
    request.key = hex_to_bytes(key_hex);
    request.iv = hex_to_bytes(iv_hex);
    request.aad = utf8_to_bytes(aad_utf8);
    request.allow_ecb = true;
    request.use_padding = true;
    request.tag_size = 16;
    return request;
}

int wrap_error(const std::exception& ex, char* error_buffer, std::size_t error_buffer_size) {
    write_c_string(error_buffer, error_buffer_size, ex.what());
    return 1;
}

} // namespace

extern "C" NT219_LAB1_API int nt219_lab1_encrypt_hex(
    const char* mode,
    const char* key_hex,
    const char* iv_hex,
    const char* plaintext_utf8,
    const char* aad_utf8,
    char* output_hex,
    size_t output_hex_size,
    char* error_buffer,
    size_t error_buffer_size) {
    try {
        auto request = make_request(mode, key_hex, iv_hex, aad_utf8);
        request.input = utf8_to_bytes(plaintext_utf8);
        const auto ciphertext = nt219::lab1::aes_encrypt(request);
        write_c_string(output_hex, output_hex_size, bytes_to_hex(ciphertext));
        write_c_string(error_buffer, error_buffer_size, "");
        return 0;
    } catch (const std::exception& ex) {
        return wrap_error(ex, error_buffer, error_buffer_size);
    } catch (...) {
        write_c_string(error_buffer, error_buffer_size, "unknown encryption error");
        return 2;
    }
}

extern "C" NT219_LAB1_API int nt219_lab1_decrypt_hex(
    const char* mode,
    const char* key_hex,
    const char* iv_hex,
    const char* ciphertext_hex,
    const char* aad_utf8,
    char* output_utf8,
    size_t output_utf8_size,
    char* error_buffer,
    size_t error_buffer_size) {
    try {
        auto request = make_request(mode, key_hex, iv_hex, aad_utf8);
        request.input = hex_to_bytes(ciphertext_hex);
        const auto plaintext = nt219::lab1::aes_decrypt(request);
        write_c_string(output_utf8, output_utf8_size, std::string(plaintext.begin(), plaintext.end()));
        write_c_string(error_buffer, error_buffer_size, "");
        return 0;
    } catch (const std::exception& ex) {
        return wrap_error(ex, error_buffer, error_buffer_size);
    } catch (...) {
        write_c_string(error_buffer, error_buffer_size, "unknown decryption error");
        return 2;
    }
}
