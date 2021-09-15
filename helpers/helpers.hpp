#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <chrono>

struct benchmark_info_t {
    double average;
};

template<typename F, typename FBefore>
[[ nodiscard ]] auto benchmark(F f, FBefore fb, int iterations) {
    using namespace std::chrono;
    double aggregate = 0;
    for (int i = 0; i < iterations; ++i) {
        fb();
        auto start = high_resolution_clock::now();
        f();
        auto end = high_resolution_clock::now();

        double ms = duration_cast<microseconds>(end - start).count() / 1000.;
        aggregate += ms;
    }
    benchmark_info_t info;
    info.average = aggregate / iterations;
    return info;
}