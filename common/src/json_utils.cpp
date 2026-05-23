#include "common/json_utils.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace nt219 {

nlohmann::json read_json_file(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);

    if (!file) {
        throw std::runtime_error("Cannot open JSON file: " + path.string());
    }

    try {
        nlohmann::json value;
        file >> value;
        return value;
    } catch (const nlohmann::json::exception& ex) {
        throw std::runtime_error("Invalid JSON file: " + path.string() + ": " + ex.what());
    }
}

void write_json_file(const std::filesystem::path& path, const nlohmann::json& value) {
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }

    std::ofstream file(path, std::ios::binary);

    if (!file) {
        throw std::runtime_error("Cannot write JSON file: " + path.string());
    }

    file << value.dump(4) << '\n';

    if (!file) {
        throw std::runtime_error("Failed while writing JSON file: " + path.string());
    }
}

std::string json_pretty_dump(const nlohmann::json& value) {
    return value.dump(4);
}

}
