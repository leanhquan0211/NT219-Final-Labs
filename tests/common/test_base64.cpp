#include "common/base64.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <stdexcept>
#include <vector>

TEST_CASE("base64 encode basic values") {
    REQUIRE(nt219::base64_encode(std::vector<std::uint8_t>{}) == "");
    REQUIRE(nt219::base64_encode(std::vector<std::uint8_t>{'f'}) == "Zg==");
    REQUIRE(nt219::base64_encode(std::vector<std::uint8_t>{'f', 'o'}) == "Zm8=");
    REQUIRE(nt219::base64_encode(std::vector<std::uint8_t>{'f', 'o', 'o'}) == "Zm9v");
}

TEST_CASE("base64 decode basic values") {
    REQUIRE(nt219::base64_decode("") == std::vector<std::uint8_t>{});
    REQUIRE(nt219::base64_decode("Zg==") == std::vector<std::uint8_t>{'f'});
    REQUIRE(nt219::base64_decode("Zm8=") == std::vector<std::uint8_t>{'f', 'o'});
    REQUIRE(nt219::base64_decode("Zm9v") == std::vector<std::uint8_t>{'f', 'o', 'o'});
}

TEST_CASE("base64 decode ignores whitespace") {
    REQUIRE(nt219::base64_decode("Z m 9 v\n") == std::vector<std::uint8_t>{'f', 'o', 'o'});
}

TEST_CASE("base64 decode rejects malformed input") {
    REQUIRE_THROWS_AS(nt219::base64_decode("Z"), std::runtime_error);
    REQUIRE_THROWS_AS(nt219::base64_decode("===="), std::runtime_error);
    REQUIRE_THROWS_AS(nt219::base64_decode("Zm=9"), std::runtime_error);
}
