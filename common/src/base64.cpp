#include "common/base64.hpp"

#include <array>
#include <cctype>
#include <stdexcept>

namespace nt219 {

std::string base64_encode(const std::vector<std::uint8_t>& data) {
    static constexpr char alphabet[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string out;
    out.reserve(((data.size() + 2) / 3) * 4);

    std::size_t i = 0;

    while (i + 2 < data.size()) {
        std::uint32_t block =
            (static_cast<std::uint32_t>(data[i]) << 16) |
            (static_cast<std::uint32_t>(data[i + 1]) << 8) |
            static_cast<std::uint32_t>(data[i + 2]);

        out.push_back(alphabet[(block >> 18) & 0x3F]);
        out.push_back(alphabet[(block >> 12) & 0x3F]);
        out.push_back(alphabet[(block >> 6) & 0x3F]);
        out.push_back(alphabet[block & 0x3F]);

        i += 3;
    }

    if (i < data.size()) {
        std::uint32_t block = static_cast<std::uint32_t>(data[i]) << 16;

        out.push_back(alphabet[(block >> 18) & 0x3F]);

        if (i + 1 < data.size()) {
            block |= static_cast<std::uint32_t>(data[i + 1]) << 8;

            out.push_back(alphabet[(block >> 12) & 0x3F]);
            out.push_back(alphabet[(block >> 6) & 0x3F]);
            out.push_back('=');
        } else {
            out.push_back(alphabet[(block >> 12) & 0x3F]);
            out.push_back('=');
            out.push_back('=');
        }
    }

    return out;
}

static std::array<int, 256> make_reverse_table() {
    std::array<int, 256> table{};
    table.fill(-1);

    const std::string alphabet =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    for (std::size_t i = 0; i < alphabet.size(); ++i) {
        table[static_cast<unsigned char>(alphabet[i])] = static_cast<int>(i);
    }

    return table;
}

std::vector<std::uint8_t> base64_decode(const std::string& text) {
    static const auto table = make_reverse_table();

    std::string cleaned;
    cleaned.reserve(text.size());

    for (char c : text) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            cleaned.push_back(c);
        }
    }

    if (cleaned.empty()) {
        return {};
    }

    if (cleaned.size() % 4 != 0) {
        throw std::runtime_error("Invalid base64 length");
    }

    std::vector<std::uint8_t> out;
    out.reserve((cleaned.size() / 4) * 3);

    for (std::size_t i = 0; i < cleaned.size(); i += 4) {
        char c0 = cleaned[i];
        char c1 = cleaned[i + 1];
        char c2 = cleaned[i + 2];
        char c3 = cleaned[i + 3];

        if (c0 == '=' || c1 == '=') {
            throw std::runtime_error("Invalid base64 padding");
        }

        int v0 = table[static_cast<unsigned char>(c0)];
        int v1 = table[static_cast<unsigned char>(c1)];
        int v2 = c2 == '=' ? 0 : table[static_cast<unsigned char>(c2)];
        int v3 = c3 == '=' ? 0 : table[static_cast<unsigned char>(c3)];

        if (v0 < 0 || v1 < 0 || (c2 != '=' && v2 < 0) || (c3 != '=' && v3 < 0)) {
            throw std::runtime_error("Invalid base64 character");
        }

        if (c2 == '=' && c3 != '=') {
            throw std::runtime_error("Invalid base64 padding");
        }

        std::uint32_t block =
            (static_cast<std::uint32_t>(v0) << 18) |
            (static_cast<std::uint32_t>(v1) << 12) |
            (static_cast<std::uint32_t>(v2) << 6) |
            static_cast<std::uint32_t>(v3);

        out.push_back(static_cast<std::uint8_t>((block >> 16) & 0xFF));

        if (c2 != '=') {
            out.push_back(static_cast<std::uint8_t>((block >> 8) & 0xFF));
        }

        if (c3 != '=') {
            out.push_back(static_cast<std::uint8_t>(block & 0xFF));
        }

        if ((c2 == '=' || c3 == '=') && i + 4 != cleaned.size()) {
            throw std::runtime_error("Base64 padding is only allowed at the end");
        }
    }

    return out;
}

}
