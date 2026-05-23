#include "common/stats.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>

namespace nt219 {

double mean(const std::vector<double>& samples) {
    if (samples.empty()) {
        throw std::runtime_error("Cannot compute mean of empty sample set");
    }

    const double sum = std::accumulate(samples.begin(), samples.end(), 0.0);
    return sum / static_cast<double>(samples.size());
}

double median(std::vector<double> samples) {
    if (samples.empty()) {
        throw std::runtime_error("Cannot compute median of empty sample set");
    }

    std::sort(samples.begin(), samples.end());

    const std::size_t n = samples.size();
    const std::size_t mid = n / 2;

    if (n % 2 == 1) {
        return samples[mid];
    }

    return (samples[mid - 1] + samples[mid]) / 2.0;
}

double sample_stddev(const std::vector<double>& samples) {
    if (samples.size() < 2) {
        return 0.0;
    }

    const double avg = mean(samples);
    double sum_squared = 0.0;

    for (double value : samples) {
        const double diff = value - avg;
        sum_squared += diff * diff;
    }

    return std::sqrt(sum_squared / static_cast<double>(samples.size() - 1));
}

StatsSummary summarize_samples(const std::vector<double>& samples) {
    if (samples.empty()) {
        throw std::runtime_error("Cannot summarize empty sample set");
    }

    StatsSummary summary;
    summary.count = samples.size();
    summary.mean = mean(samples);
    summary.median = median(samples);
    summary.stddev = sample_stddev(samples);

    const auto [min_it, max_it] = std::minmax_element(samples.begin(), samples.end());
    summary.min = *min_it;
    summary.max = *max_it;

    const double margin =
        samples.size() > 1
            ? 1.96 * summary.stddev / std::sqrt(static_cast<double>(samples.size()))
            : 0.0;

    summary.ci95_low = summary.mean - margin;
    summary.ci95_high = summary.mean + margin;

    return summary;
}

}
