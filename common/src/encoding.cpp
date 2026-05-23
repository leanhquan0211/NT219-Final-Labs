#include "common/encoding.hpp"

#include "common/base64.hpp"
#include "common/hex.hpp"

#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace nt219 {

static std::string to_lower_copy(std::string value) {
    std::transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });

    return value;
}

Encoding parse_encoding(const std::string& value) {
    const std::string normalized = to_lower_copy(value);

    if (normalized == "raw") {
        return Encoding::Raw;
    }

    if (normalized == "hex") {
        return Encoding::Hex;
    }

    if (normalized == "base64" || normalized == "b64") {
        return Encoding::Base64;
    }

    throw std::runtime_error("Unsupported encoding: " + value);
}

std::string encoding_to_string(Encoding encoding) {
    switch (encoding) {
        case Encoding::Raw:
            return "raw";
        case Encoding::Hex:
            return "hex";
        case Encoding::Base64:
            return "base64";
    }

    throw std::runtime_error("Unknown encoding");
}

std::string encode_bytes(const std::vector<std::uint8_t>& data, Encoding encoding) {
    switch (encoding) {
        case Encoding::Raw:
            return std::string(data.begin(), data.end());

        case Encoding::Hex:
            return hex_encode(data);

        case Encoding::Base64:
            return base64_encode(data);
    }

    throw std::runtime_error("Unknown encoding");
}

std::vector<std::uint8_t> decode_to_bytes(const std::string& text, Encoding encoding) {
    switch (encoding) {
        case Encoding::Raw:
            return std::vector<std::uint8_t>(text.begin(), text.end());

        case Encoding::Hex:
            return hex_decode(text);

        case Encoding::Base64:
            return base64_decode(text);
    }

    throw std::runtime_error("Unknown encoding");
}

}
