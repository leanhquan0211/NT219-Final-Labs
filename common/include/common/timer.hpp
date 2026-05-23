#pragma once

#include <chrono>

namespace nt219 {

class Timer {
public:
    Timer();

    void reset();
    double elapsed_ms() const;

private:
    std::chrono::steady_clock::time_point start_;
};

}
