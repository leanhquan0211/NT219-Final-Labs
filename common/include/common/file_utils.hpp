#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace nt219 {

std::vector<std::uint8_t> read_binary_file(const std::string& path);
void write_binary_file(const std::string& path, const std::vector<std::uint8_t>& data);

std::string read_text_file(const std::string& path);
void write_text_file(const std::string& path, const std::string& data);

}
