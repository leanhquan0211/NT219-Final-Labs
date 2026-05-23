#include "common/sample_generator.hpp"

#include <catch2/catch_test_macros.hpp>

#include <filesystem>

TEST_CASE("sample generator returns default benchmark specs") {
    const auto specs = nt219::default_benchmark_sample_specs();

    REQUIRE(specs.size() == 6);
    REQUIRE(specs[0].filename == "sample_1KiB.bin");
    REQUIRE(specs[0].size_bytes == 1024);
    REQUIRE(specs[5].filename == "sample_8MiB.bin");
    REQUIRE(specs[5].size_bytes == 8 * 1024 * 1024);
}

TEST_CASE("sample generator creates deterministic data") {
    const auto a = nt219::generate_deterministic_sample(64);
    const auto b = nt219::generate_deterministic_sample(64);

    REQUIRE(a.size() == 64);
    REQUIRE(b.size() == 64);
    REQUIRE(a == b);
}

TEST_CASE("sample generator can create benchmark files") {
    const auto output_dir =
        std::filesystem::temp_directory_path() / "nt219_sample_generator_test";

    nt219::generate_default_benchmark_samples(output_dir);

    REQUIRE(std::filesystem::exists(output_dir / "sample_1KiB.bin"));
    REQUIRE(std::filesystem::exists(output_dir / "sample_4KiB.bin"));
    REQUIRE(std::filesystem::exists(output_dir / "sample_16KiB.bin"));
    REQUIRE(std::filesystem::exists(output_dir / "sample_256KiB.bin"));
    REQUIRE(std::filesystem::exists(output_dir / "sample_1MiB.bin"));
    REQUIRE(std::filesystem::exists(output_dir / "sample_8MiB.bin"));

    REQUIRE(std::filesystem::file_size(output_dir / "sample_1KiB.bin") == 1024);
    REQUIRE(std::filesystem::file_size(output_dir / "sample_8MiB.bin") == 8 * 1024 * 1024);

    std::filesystem::remove_all(output_dir);
}
