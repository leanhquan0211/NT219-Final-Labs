#include "common/sample_generator.hpp"

#include "common/file_utils.hpp"

#include <filesystem>

namespace nt219 {

std::vector<SampleFileSpec> default_benchmark_sample_specs() {
    return {
        {"sample_1KiB.bin", 1024},
        {"sample_4KiB.bin", 4 * 1024},
        {"sample_16KiB.bin", 16 * 1024},
        {"sample_256KiB.bin", 256 * 1024},
        {"sample_1MiB.bin", 1024 * 1024},
        {"sample_8MiB.bin", 8 * 1024 * 1024}
    };
}

std::vector<std::uint8_t> generate_deterministic_sample(std::size_t size_bytes) {
    std::vector<std::uint8_t> data(size_bytes);

    std::uint32_t state = 0x4E543231U;

    for (std::size_t i = 0; i < data.size(); ++i) {
        state = state * 1664525U + 1013904223U;
        data[i] = static_cast<std::uint8_t>((state >> 24) & 0xFF);
    }

    return data;
}

void generate_default_benchmark_samples(const std::filesystem::path& output_directory) {
    std::filesystem::create_directories(output_directory);

    for (const auto& spec : default_benchmark_sample_specs()) {
        const auto path = output_directory / spec.filename;
        const auto data = generate_deterministic_sample(spec.size_bytes);
        write_binary_file(path.string(), data);
    }
}

}
