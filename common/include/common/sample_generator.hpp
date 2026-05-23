#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace nt219 {

struct SampleFileSpec {
    std::string filename;
    std::size_t size_bytes;
};

std::vector<SampleFileSpec> default_benchmark_sample_specs();

std::vector<std::uint8_t> generate_deterministic_sample(std::size_t size_bytes);

void generate_default_benchmark_samples(const std::filesystem::path& output_directory);

}
