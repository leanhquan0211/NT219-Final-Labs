#include "common/hex.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <vector>

TEST_CASE("hex encode lower-case output") {
    std::vector<std::uint8_t> data = {0x00, 0x01, 0xAB, 0xCD, 0xEF};
    REQUIRE(nt219::hex_encode(data) == "0001abcdef");
}

TEST_CASE("hex decode roundtrip") {
    auto data = nt219::hex_decode("0001abcdef");
    REQUIRE(data == std::vector<std::uint8_t>{0x00, 0x01, 0xAB, 0xCD, 0xEF});
}
