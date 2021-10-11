#pragma once
#include <functional>
#include <omp.h>
#include <helpers/helpers.hpp>

template <typename T>
T integral_trapezoidal_sync(std::function<T(T)> f, T a, T b, unsigned int intervals) {
    T width = (b - a) / T(intervals);
    T sum = 0;
    T x0 = a, x1 = a + width;
    for (; x1 <= b+T(0.0000001); x0 = x1, x1 += width) {
        T y0 = f(x0), y1 = f(x1);
        sum += (y1 + y0) / 2 * width;
    }
    return sum;
}

template <typename T>
T integral_trapezoidal_omp(std::function<T(T)> f, T a, T b, unsigned int intervals) {
    T width = (b - a) / T(intervals);
    T sum = 0;
    auto partition = distribute::partition_intervals(intervals, omp_get_max_threads());
    #pragma omp parallel default(none) shared(intervals, sum, width, f, a, b, partition) num_threads(partition.threads)
    {
        auto part = distribute::omp::partition_get_i(partition);
        T x0 = a + part.i * width;
        T x1 = x0 + width;

        // #pragma omp critical
        // {
        //     // some error
        //     printf("%d %lf->%lf\n", omp_get_thread_num(), x0, x0 + part.size * width);
        //     // printf("local sum %lf\n", local_sum);
        // }

        T local_sum = 0;
        for (int i = 0; i < part.size; ++i, x0 = x1, x1 += width) {
            T y0 = f(x0), y1 = f(x1);
            local_sum += (y1 + y0) / 2 * width;
        }

        #pragma omp atomic update
        sum += local_sum;
    }
    return sum;
}
