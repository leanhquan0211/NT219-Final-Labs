#include "common/json_utils.hpp"

#include <catch2/catch_test_macros.hpp>

#include <filesystem>

TEST_CASE("json utils write and read json file") {
    const auto temp_path =
        std::filesystem::temp_directory_path() / "nt219_json_utils_test.json";

    nlohmann::json original;
    original["tool"] = "aestool";
    original["mode"] = "gcm";
    original["key_bits"] = 256;

    nt219::write_json_file(temp_path, original);

    const auto loaded = nt219::read_json_file(temp_path);

    REQUIRE(loaded["tool"] == "aestool");
    REQUIRE(loaded["mode"] == "gcm");
    REQUIRE(loaded["key_bits"] == 256);

    std::filesystem::remove(temp_path);
}

TEST_CASE("json pretty dump returns indented json") {
    nlohmann::json value;
    value["lab"] = 1;
    value["name"] = "AES Crypto++";

    const auto dumped = nt219::json_pretty_dump(value);

    REQUIRE(dumped.find("\"lab\"") != std::string::npos);
    REQUIRE(dumped.find("\"name\"") != std::string::npos);
}
