#include "common/encoding.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <stdexcept>
#include <vector>

TEST_CASE("parse encoding supports raw hex base64") {
    REQUIRE(nt219::parse_encoding("raw") == nt219::Encoding::Raw);
    REQUIRE(nt219::parse_encoding("hex") == nt219::Encoding::Hex);
    REQUIRE(nt219::parse_encoding("base64") == nt219::Encoding::Base64);
    REQUIRE(nt219::parse_encoding("b64") == nt219::Encoding::Base64);
}

TEST_CASE("parse encoding is case insensitive") {
    REQUIRE(nt219::parse_encoding("RAW") == nt219::Encoding::Raw);
    REQUIRE(nt219::parse_encoding("Hex") == nt219::Encoding::Hex);
    REQUIRE(nt219::parse_encoding("BASE64") == nt219::Encoding::Base64);
}

TEST_CASE("parse encoding rejects unsupported value") {
    REQUIRE_THROWS_AS(nt219::parse_encoding("pem"), std::runtime_error);
}

TEST_CASE("encode and decode bytes with hex") {
    const std::vector<std::uint8_t> data = {0x00, 0x01, 0xAB, 0xCD, 0xEF};

    const std::string encoded = nt219::encode_bytes(data, nt219::Encoding::Hex);
    REQUIRE(encoded == "0001abcdef");

    const auto decoded = nt219::decode_to_bytes(encoded, nt219::Encoding::Hex);
    REQUIRE(decoded == data);
}

TEST_CASE("encode and decode bytes with base64") {
    const std::vector<std::uint8_t> data = {'f', 'o', 'o'};

    const std::string encoded = nt219::encode_bytes(data, nt219::Encoding::Base64);
    REQUIRE(encoded == "Zm9v");

    const auto decoded = nt219::decode_to_bytes(encoded, nt219::Encoding::Base64);
    REQUIRE(decoded == data);
}

TEST_CASE("encode and decode bytes with raw") {
    const std::vector<std::uint8_t> data = {'h', 'e', 'l', 'l', 'o'};

    const std::string encoded = nt219::encode_bytes(data, nt219::Encoding::Raw);
    REQUIRE(encoded == "hello");

    const auto decoded = nt219::decode_to_bytes(encoded, nt219::Encoding::Raw);
    REQUIRE(decoded == data);
}
