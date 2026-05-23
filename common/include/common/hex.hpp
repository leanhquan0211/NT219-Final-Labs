#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace nt219 {

std::string hex_encode(const std::vector<std::uint8_t>& data);
std::vector<std::uint8_t> hex_decode(const std::string& hex);

}
