#include "common/base64.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <vector>

TEST_CASE("base64 encode basic values") {
    REQUIRE(nt219::base64_encode(std::vector<std::uint8_t>{}) == "");
    REQUIRE(nt219::base64_encode(std::vector<std::uint8_t>{'f'}) == "Zg==");
    REQUIRE(nt219::base64_encode(std::vector<std::uint8_t>{'f', 'o'}) == "Zm8=");
    REQUIRE(nt219::base64_encode(std::vector<std::uint8_t>{'f', 'o', 'o'}) == "Zm9v");
}
