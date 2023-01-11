#pragma once

#include <cstdint>
#include <stdexcept>
#include <iostream>

namespace kmonitor_adapter {

namespace detail {

inline double getCpuMhz() {
    FILE *f;
    char buf[256];
    double mhz = 0.0;

    f = ::fopen("/proc/cpuinfo", "r");
    if (!f) return 0.0;
    while (::fgets(buf, sizeof(buf), f)) {
        double m;
        int rc;
        rc = ::sscanf(buf, "cpu MHz : %lf", &m);
        if (rc == 1 && mhz == 0.0) {
            mhz = m;
            break;
        }
    }
    ::fclose(f);
    return mhz;
}

static const double SPEED_FACTOR = 1.0 / getCpuMhz();

}  // namespace detail

namespace time {

inline uint64_t getCycles() {
    uint32_t lo, hi;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

inline uint64_t getMicroSeconds(uint64_t cycles) { return cycles * detail::SPEED_FACTOR; }

class Point {
public:
    Point() = default;
    Point(uint64_t cycles) : _cycles(cycles) {}

    uint64_t cycles() const noexcept { return _cycles; }

private:
    uint64_t _cycles = 0;
};

class Duration {
public:
    Duration() = default;
    Duration(uint64_t cycles) : _cycles(cycles) {}

    uint64_t cycles() const noexcept { return _cycles; }
    uint64_t toMicroSeconds() const noexcept { return _cycles * detail::SPEED_FACTOR; }

private:
    uint64_t _cycles = 0;
};

inline Duration operator-(const Point& lhs, const Point& rhs) {
    return Duration(lhs.cycles() - rhs.cycles());
}

}  // namespace time

}  // namespace kmonitor_adapter
