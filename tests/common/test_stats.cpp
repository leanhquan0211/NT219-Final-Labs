#include "common/stats.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <stdexcept>
#include <vector>

TEST_CASE("stats compute mean median and stddev") {
    const std::vector<double> samples = {1.0, 2.0, 3.0, 4.0, 5.0};

    REQUIRE(nt219::mean(samples) == Catch::Approx(3.0));
    REQUIRE(nt219::median(samples) == Catch::Approx(3.0));
    REQUIRE(nt219::sample_stddev(samples) == Catch::Approx(1.58113883008));
}

TEST_CASE("stats median supports even sample count") {
    const std::vector<double> samples = {1.0, 2.0, 3.0, 4.0};

    REQUIRE(nt219::median(samples) == Catch::Approx(2.5));
}

TEST_CASE("stats summary includes confidence interval") {
    const std::vector<double> samples = {10.0, 12.0, 14.0, 16.0, 18.0};

    const auto summary = nt219::summarize_samples(samples);

    REQUIRE(summary.count == 5);
    REQUIRE(summary.mean == Catch::Approx(14.0));
    REQUIRE(summary.median == Catch::Approx(14.0));
    REQUIRE(summary.min == Catch::Approx(10.0));
    REQUIRE(summary.max == Catch::Approx(18.0));
    REQUIRE(summary.ci95_low < summary.mean);
    REQUIRE(summary.ci95_high > summary.mean);
}

TEST_CASE("stats rejects empty samples") {
    REQUIRE_THROWS_AS(nt219::mean({}), std::runtime_error);
    REQUIRE_THROWS_AS(nt219::median({}), std::runtime_error);
    REQUIRE_THROWS_AS(nt219::summarize_samples({}), std::runtime_error);
}
