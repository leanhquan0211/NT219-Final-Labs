#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace nt219 {

enum class Encoding {
    Raw,
    Hex,
    Base64
};

Encoding parse_encoding(const std::string& value);

std::string encoding_to_string(Encoding encoding);

std::string encode_bytes(const std::vector<std::uint8_t>& data, Encoding encoding);

std::vector<std::uint8_t> decode_to_bytes(const std::string& text, Encoding encoding);

}
