#include "common/hex.hpp"

#include <cctype>
#include <stdexcept>

namespace nt219 {

std::string hex_encode(const std::vector<std::uint8_t>& data) {
    static constexpr char alphabet[] = "0123456789abcdef";

    std::string out;
    out.reserve(data.size() * 2);

    for (std::uint8_t byte : data) {
        out.push_back(alphabet[(byte >> 4) & 0x0F]);
        out.push_back(alphabet[byte & 0x0F]);
    }

    return out;
}

static std::uint8_t hex_value(char c) {
    if (c >= '0' && c <= '9') {
        return static_cast<std::uint8_t>(c - '0');
    }
    if (c >= 'a' && c <= 'f') {
        return static_cast<std::uint8_t>(c - 'a' + 10);
    }
    if (c >= 'A' && c <= 'F') {
        return static_cast<std::uint8_t>(c - 'A' + 10);
    }
    throw std::runtime_error("Invalid hex character");
}

std::vector<std::uint8_t> hex_decode(const std::string& hex) {
    std::string cleaned;
    cleaned.reserve(hex.size());

    for (char c : hex) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            cleaned.push_back(c);
        }
    }

    if (cleaned.size() % 2 != 0) {
        throw std::runtime_error("Hex string length must be even");
    }

    std::vector<std::uint8_t> out;
    out.reserve(cleaned.size() / 2);

    for (std::size_t i = 0; i < cleaned.size(); i += 2) {
        std::uint8_t high = hex_value(cleaned[i]);
        std::uint8_t low = hex_value(cleaned[i + 1]);
        out.push_back(static_cast<std::uint8_t>((high << 4) | low));
    }

    return out;
}

}
