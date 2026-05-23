#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>

namespace nt219 {

nlohmann::json read_json_file(const std::filesystem::path& path);

void write_json_file(const std::filesystem::path& path, const nlohmann::json& value);

std::string json_pretty_dump(const nlohmann::json& value);

}
