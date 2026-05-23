#include "common/timer.hpp"

namespace nt219 {

Timer::Timer() {
    reset();
}

void Timer::reset() {
    start_ = std::chrono::steady_clock::now();
}

double Timer::elapsed_ms() const {
    const auto end = std::chrono::steady_clock::now();
    const auto diff = std::chrono::duration<double, std::milli>(end - start_);
    return diff.count();
}

}
