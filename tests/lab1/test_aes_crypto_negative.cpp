#include "lab1/aes_crypto.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <stdexcept>
#include <vector>

TEST_CASE("Lab 1 GCM rejects tampered ciphertext") {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::AesMode::Gcm;
    request.key = std::vector<std::uint8_t>(32, 0x11);
    request.iv = std::vector<std::uint8_t>(12, 0x22);
    request.aad = {'m', 'e', 't', 'a'};
    request.input = {'h', 'e', 'l', 'l', 'o'};
    request.tag_size = 16;

    auto ciphertext = nt219::lab1::aes_encrypt(request);
    ciphertext[0] ^= 0x01;

    request.input = ciphertext;
    REQUIRE_THROWS_AS(nt219::lab1::aes_decrypt(request), std::exception);
}

TEST_CASE("Lab 1 rejects invalid AES key lengths") {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::AesMode::Ctr;
    request.key = std::vector<std::uint8_t>(15, 0x00);
    request.iv = std::vector<std::uint8_t>(16, 0x00);
    request.input = {'x'};

    REQUIRE_THROWS_AS(nt219::lab1::aes_encrypt(request), std::runtime_error);
}

TEST_CASE("Lab 1 rejects invalid IV lengths") {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::AesMode::Cbc;
    request.key = std::vector<std::uint8_t>(16, 0x00);
    request.iv = std::vector<std::uint8_t>(15, 0x00);
    request.input = {'x'};

    REQUIRE_THROWS_AS(nt219::lab1::aes_encrypt(request), std::runtime_error);
}

TEST_CASE("Lab 1 rejects AAD for non-AEAD modes") {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::AesMode::Ctr;
    request.key = std::vector<std::uint8_t>(16, 0x00);
    request.iv = std::vector<std::uint8_t>(16, 0x00);
    request.aad = {'a', 'a', 'd'};
    request.input = {'x'};

    REQUIRE_THROWS_AS(nt219::lab1::aes_encrypt(request), std::runtime_error);
}

TEST_CASE("Lab 1 blocks large ECB input unless explicitly allowed") {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::AesMode::Ecb;
    request.key = std::vector<std::uint8_t>(16, 0x00);
    request.input = std::vector<std::uint8_t>(20 * 1024, 0x41);

    REQUIRE_THROWS_AS(nt219::lab1::aes_encrypt(request), std::runtime_error);

    request.allow_ecb = true;
    REQUIRE_NOTHROW(nt219::lab1::aes_encrypt(request));
}

TEST_CASE("Lab 1 currently reports XTS as reserved") {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::AesMode::Xts;
    request.key = std::vector<std::uint8_t>(32, 0x00);
    request.iv = std::vector<std::uint8_t>(16, 0x00);
    request.input = std::vector<std::uint8_t>(32, 0x41);

    REQUIRE_THROWS_AS(nt219::lab1::aes_encrypt(request), std::runtime_error);
}
