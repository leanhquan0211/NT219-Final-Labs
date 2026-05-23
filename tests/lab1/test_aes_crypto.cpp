#include "lab1/aes_crypto.hpp"
#include "common/hex.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <stdexcept>
#include <vector>

TEST_CASE("Lab 1 AES CBC round trip") {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::AesMode::Cbc;
    request.key = nt219::hex_decode("2b7e151628aed2a6abf7158809cf4f3c");
    request.iv = nt219::hex_decode("000102030405060708090a0b0c0d0e0f");
    request.input = std::vector<std::uint8_t>{'N','T','2','1','9'};
    const auto ciphertext = nt219::lab1::aes_encrypt(request);
    request.input = ciphertext;
    REQUIRE(nt219::lab1::aes_decrypt(request) == std::vector<std::uint8_t>({'N','T','2','1','9'}));
}

TEST_CASE("Lab 1 AES CTR round trip with partial block") {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::AesMode::Ctr;
    request.key = nt219::hex_decode("2b7e151628aed2a6abf7158809cf4f3c");
    request.iv = nt219::hex_decode("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff");
    request.input = std::vector<std::uint8_t>{'p','a','r','t','i','a','l'};
    const auto ciphertext = nt219::lab1::aes_encrypt(request);
    request.input = ciphertext;
    REQUIRE(nt219::lab1::aes_decrypt(request) == std::vector<std::uint8_t>({'p','a','r','t','i','a','l'}));
}

TEST_CASE("Lab 1 AES GCM detects tampering") {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::AesMode::Gcm;
    request.key = std::vector<std::uint8_t>(32, 0x11);
    request.iv = std::vector<std::uint8_t>(12, 0x22);
    request.aad = std::vector<std::uint8_t>{'a','a','d'};
    request.input = std::vector<std::uint8_t>{'h','e','l','l','o'};
    auto ciphertext = nt219::lab1::aes_encrypt(request);
    request.input = ciphertext;
    REQUIRE(nt219::lab1::aes_decrypt(request) == std::vector<std::uint8_t>({'h','e','l','l','o'}));
    ciphertext[0] ^= 0x01;
    request.input = ciphertext;
    REQUIRE_THROWS_AS(nt219::lab1::aes_decrypt(request), std::exception);
}

TEST_CASE("Lab 1 rejects invalid key size") {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::AesMode::Cbc;
    request.key = std::vector<std::uint8_t>(15, 0x00);
    request.iv = std::vector<std::uint8_t>(16, 0x00);
    request.input = std::vector<std::uint8_t>{'x'};
    REQUIRE_THROWS_AS(nt219::lab1::aes_encrypt(request), std::runtime_error);
}

TEST_CASE("Lab 1 rejects invalid IV size") {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::AesMode::Gcm;
    request.key = std::vector<std::uint8_t>(32, 0x00);
    request.iv = std::vector<std::uint8_t>(16, 0x00);
    request.input = std::vector<std::uint8_t>{'x'};
    REQUIRE_THROWS_AS(nt219::lab1::aes_encrypt(request), std::runtime_error);
}

TEST_CASE("Lab 1 selftest passes") {
    REQUIRE(nt219::lab1::run_lab1_basic_selftest());
}
