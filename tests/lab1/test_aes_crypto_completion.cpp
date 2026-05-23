#include "lab1/aes_crypto.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <stdexcept>
#include <vector>

TEST_CASE("Lab 1 AES-CBC supports no-padding NIST-style block encryption") {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::AesMode::Cbc;
    request.key = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
    request.iv = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    request.input = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a};
    request.use_padding = false;
    const auto ciphertext = nt219::lab1::aes_encrypt(request);
    const std::vector<std::uint8_t> expected = {0x76,0x49,0xab,0xac,0x81,0x19,0xb2,0x46,0xce,0xe9,0x8e,0x9b,0x12,0xe9,0x19,0x7d};
    REQUIRE(ciphertext == expected);
    request.input = ciphertext;
    REQUIRE(nt219::lab1::aes_decrypt(request) == std::vector<std::uint8_t>({0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a}));
}

TEST_CASE("Lab 1 AES-XTS round trip works for sector-sized data") {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::AesMode::Xts;
    request.key = std::vector<std::uint8_t>(32, 0x42);
    request.iv = std::vector<std::uint8_t>(16, 0x24);
    request.input = std::vector<std::uint8_t>(64);
    for (std::size_t i = 0; i < request.input.size(); ++i) {
        request.input[i] = static_cast<std::uint8_t>(i);
    }
    const auto plaintext = request.input;
    const auto ciphertext = nt219::lab1::aes_encrypt(request);
    REQUIRE(ciphertext.size() == plaintext.size());
    REQUIRE(ciphertext != plaintext);
    request.input = ciphertext;
    REQUIRE(nt219::lab1::aes_decrypt(request) == plaintext);
}

TEST_CASE("Lab 1 AES-XTS rejects invalid key sizes") {
    REQUIRE_THROWS_AS(nt219::lab1::validate_aes_key(std::vector<std::uint8_t>(16, 0x00), nt219::lab1::AesMode::Xts), std::runtime_error);
    REQUIRE_NOTHROW(nt219::lab1::validate_aes_key(std::vector<std::uint8_t>(32, 0x00), nt219::lab1::AesMode::Xts));
    REQUIRE_NOTHROW(nt219::lab1::validate_aes_key(std::vector<std::uint8_t>(64, 0x00), nt219::lab1::AesMode::Xts));
}

TEST_CASE("Lab 1 identifies nonce-reuse sensitive modes") {
    REQUIRE(nt219::lab1::is_nonce_reuse_sensitive_mode(nt219::lab1::AesMode::Ctr));
    REQUIRE(nt219::lab1::is_nonce_reuse_sensitive_mode(nt219::lab1::AesMode::Gcm));
    REQUIRE(nt219::lab1::is_nonce_reuse_sensitive_mode(nt219::lab1::AesMode::Ccm));
    REQUIRE_FALSE(nt219::lab1::is_nonce_reuse_sensitive_mode(nt219::lab1::AesMode::Cbc));
    REQUIRE_FALSE(nt219::lab1::is_nonce_reuse_sensitive_mode(nt219::lab1::AesMode::Xts));
}
