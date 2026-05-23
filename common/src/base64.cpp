#include "common/base64.hpp"

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

}
