#include "common/random.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("openssl random bytes returns requested size") {
    const auto data = nt219::random_bytes_openssl(32);

    REQUIRE(data.size() == 32);
}

TEST_CASE("cryptopp random bytes returns requested size") {
    const auto data = nt219::random_bytes_cryptopp(32);

    REQUIRE(data.size() == 32);
}

TEST_CASE("random bytes supports zero length") {
    REQUIRE(nt219::random_bytes_openssl(0).empty());
    REQUIRE(nt219::random_bytes_cryptopp(0).empty());
}
