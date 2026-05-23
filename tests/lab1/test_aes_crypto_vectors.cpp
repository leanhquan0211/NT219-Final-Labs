#include "lab1/aes_crypto.hpp"
#include "common/hex.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <vector>

namespace {

std::vector<std::uint8_t> hx(const char* text) {
    return nt219::hex_decode(text);
}

} // namespace

TEST_CASE("Lab 1 AES-CTR matches NIST SP 800-38A multi-block vector") {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::AesMode::Ctr;
    request.key = hx("2b7e151628aed2a6abf7158809cf4f3c");
    request.iv = hx("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff");
    request.input = hx(
        "6bc1bee22e409f96e93d7e117393172a"
        "ae2d8a571e03ac9c9eb76fac45af8e51"
        "30c81c46a35ce411e5fbc1191a0a52ef"
        "f69f2445df4f9b17ad2b417be66c3710");

    const auto ciphertext = nt219::lab1::aes_encrypt(request);

    REQUIRE(nt219::hex_encode(ciphertext) ==
        "874d6191b620e3261bef6864990db6ce"
        "9806f66b7970fdff8617187bb9fffdff"
        "5ae4df3edbd5d35e5b4f09020db03eab"
        "1e031dda2fbe03d1792170a0f3009cee");

    request.input = ciphertext;
    const auto recovered = nt219::lab1::aes_decrypt(request);
    REQUIRE(recovered == hx(
        "6bc1bee22e409f96e93d7e117393172a"
        "ae2d8a571e03ac9c9eb76fac45af8e51"
        "30c81c46a35ce411e5fbc1191a0a52ef"
        "f69f2445df4f9b17ad2b417be66c3710"));
}

TEST_CASE("Lab 1 AES-OFB matches NIST SP 800-38A multi-block vector") {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::AesMode::Ofb;
    request.key = hx("2b7e151628aed2a6abf7158809cf4f3c");
    request.iv = hx("000102030405060708090a0b0c0d0e0f");
    request.input = hx(
        "6bc1bee22e409f96e93d7e117393172a"
        "ae2d8a571e03ac9c9eb76fac45af8e51"
        "30c81c46a35ce411e5fbc1191a0a52ef"
        "f69f2445df4f9b17ad2b417be66c3710");

    const auto ciphertext = nt219::lab1::aes_encrypt(request);

    REQUIRE(nt219::hex_encode(ciphertext) ==
        "3b3fd92eb72dad20333449f8e83cfb4a"
        "7789508d16918f03f53c52dac54ed825"
        "9740051e9c5fecf64344f7a82260edcc"
        "304c6528f659c77866a510d9c1d6ae5e");

    request.input = ciphertext;
    const auto recovered = nt219::lab1::aes_decrypt(request);
    REQUIRE(recovered == hx(
        "6bc1bee22e409f96e93d7e117393172a"
        "ae2d8a571e03ac9c9eb76fac45af8e51"
        "30c81c46a35ce411e5fbc1191a0a52ef"
        "f69f2445df4f9b17ad2b417be66c3710"));
}

TEST_CASE("Lab 1 AES-CBC round trip works with padded file-style input") {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::AesMode::Cbc;
    request.key = hx("2b7e151628aed2a6abf7158809cf4f3c");
    request.iv = hx("000102030405060708090a0b0c0d0e0f");
    request.input = hx(
        "6bc1bee22e409f96e93d7e117393172a"
        "ae2d8a571e03ac9c9eb76fac45af8e51"
        "30c81c46a35ce411e5fbc1191a0a52ef"
        "f69f2445df4f9b17ad2b417be66c3710");

    const auto ciphertext = nt219::lab1::aes_encrypt(request);
    REQUIRE(ciphertext.size() == request.input.size() + 16);

    const auto original = request.input;
    request.input = ciphertext;
    const auto recovered = nt219::lab1::aes_decrypt(request);
    REQUIRE(recovered == original);
}

TEST_CASE("Lab 1 AES-CFB round trip works with arbitrary-length input") {
    nt219::lab1::AesRequest request;
    request.mode = nt219::lab1::AesMode::Cfb;
    request.key = hx("2b7e151628aed2a6abf7158809cf4f3c");
    request.iv = hx("000102030405060708090a0b0c0d0e0f");
    request.input = hx(
        "6bc1bee22e409f96e93d7e117393172a"
        "ae2d8a571e03ac9c9eb76fac45af8e51"
        "30c81c46a35ce411e5fbc1191a0a52ef"
        "f69f2445df4f9b17ad2b417be66c3710"
        "aabbcc");

    const auto original = request.input;
    const auto ciphertext = nt219::lab1::aes_encrypt(request);
    REQUIRE(ciphertext.size() == original.size());

    request.input = ciphertext;
    const auto recovered = nt219::lab1::aes_decrypt(request);
    REQUIRE(recovered == original);
}
