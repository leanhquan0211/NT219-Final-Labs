#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace nt219::lab1 {

enum class AesMode {
    Ecb,
    Cbc,
    Cfb,
    Ofb,
    Ctr,
    Gcm,
    Ccm,
    Xts
};

struct AesRequest {
    AesMode mode{};
    std::vector<std::uint8_t> key;
    std::vector<std::uint8_t> iv;
    std::vector<std::uint8_t> aad;
    std::vector<std::uint8_t> input;
    bool allow_ecb{false};
    std::size_t tag_size{16};
};

AesMode parse_aes_mode(const std::string& value);
std::string aes_mode_to_string(AesMode mode);
bool is_aead_mode(AesMode mode);
bool mode_requires_iv(AesMode mode);
std::size_t required_iv_size(AesMode mode);
void validate_aes_key(const std::vector<std::uint8_t>& key, AesMode mode);
void validate_iv(const std::vector<std::uint8_t>& iv, AesMode mode);
std::vector<std::uint8_t> aes_encrypt(const AesRequest& request);
std::vector<std::uint8_t> aes_decrypt(const AesRequest& request);
bool run_lab1_basic_selftest();

}
