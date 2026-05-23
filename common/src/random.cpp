#include "common/random.hpp"

#include <cryptopp/osrng.h>
#include <openssl/rand.h>

#include <limits>
#include <stdexcept>

namespace nt219 {

std::vector<std::uint8_t> random_bytes_openssl(std::size_t size) {
    if (size > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::runtime_error("Requested random byte count is too large for OpenSSL RAND_bytes");
    }

    std::vector<std::uint8_t> data(size);

    if (!data.empty()) {
        const int ok = RAND_bytes(data.data(), static_cast<int>(data.size()));

        if (ok != 1) {
            throw std::runtime_error("OpenSSL RAND_bytes failed");
        }
    }

    return data;
}

std::vector<std::uint8_t> random_bytes_cryptopp(std::size_t size) {
    std::vector<std::uint8_t> data(size);

    if (!data.empty()) {
        CryptoPP::AutoSeededRandomPool rng;
        rng.GenerateBlock(data.data(), data.size());
    }

    return data;
}

}
