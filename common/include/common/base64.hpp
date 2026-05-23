#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace nt219 {

std::string base64_encode(const std::vector<std::uint8_t>& data);

std::vector<std::uint8_t> base64_decode(const std::string& text);

}
