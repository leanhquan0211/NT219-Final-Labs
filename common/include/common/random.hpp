#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace nt219 {

std::vector<std::uint8_t> random_bytes_openssl(std::size_t size);

std::vector<std::uint8_t> random_bytes_cryptopp(std::size_t size);

}
