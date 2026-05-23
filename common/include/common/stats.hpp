#pragma once

#include <cstddef>
#include <vector>

namespace nt219 {

struct StatsSummary {
    std::size_t count{};
    double mean{};
    double median{};
    double stddev{};
    double ci95_low{};
    double ci95_high{};
    double min{};
    double max{};
};

StatsSummary summarize_samples(const std::vector<double>& samples);

double mean(const std::vector<double>& samples);

double median(std::vector<double> samples);

double sample_stddev(const std::vector<double>& samples);

}
