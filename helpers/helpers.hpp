#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#define USE_OMP
#include <chrono>

struct benchmark_info_t {
    double average;
};

template <typename F, typename FBefore>
[[nodiscard]] auto benchmark(F f, FBefore fb, int iterations) {
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

template <typename F>
[[nodiscard]] auto benchmark(F f, int iterations) {
    return benchmark(
        f, []() {}, iterations);
}

template <typename F>
[[nodiscard]] auto benchmark(F f) {
    return benchmark(
        f, []() {}, 1);
}

template <typename T>
T readv(const char *msg);

namespace distribute {

struct invls_partition {
    int threads;
    int partition_size;
    int ext_partitions; // partitions of size = partition_size + 1
    int intervals;
};

struct invl_part {
    int i;
    int size;
};

invls_partition partition_intervals(int intervals, int max_threads) noexcept;
invl_part partition_get_i(const invls_partition& partition, int tid) noexcept;

#ifdef USE_OMP
namespace omp {

// invls_partition partition_intervals(int intervals) noexcept;
invl_part partition_get_i(const invls_partition& partition) noexcept;

}
#endif

} // namespace distribute

// int read_int(const char* msg);
// float read_float(const char* msg);