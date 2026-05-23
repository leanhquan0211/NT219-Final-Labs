#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace nt219 {

std::string base64_encode(const std::vector<std::uint8_t>& data);

}
