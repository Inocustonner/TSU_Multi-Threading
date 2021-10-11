#pragma once
#include <functional>
#include <omp.h>

template<typename T>
T simpson_method_sync(std::function<T(T)> f, T a, T b, unsigned int n) {
    auto h = (b - a) / T(n);
    T sum1 = 0; T sum2 = 0;
    for (auto k = 1u; k <= n; ++k) {
        auto xk = a + k * h;
        if (k <= n - 1)
            sum1 += f(xk);
        auto xk_1 = a + (k - 1) * h;
        sum2 += f((xk + xk_1) / 2);
    }
    T half = T(1) / T(2);
    return h / T(3) * (half * f(a) + sum1 + 2 * sum2 + half * f(b));
}

template<typename T>
T simpson_method_omp(std::function<T(T)> f, T a, T b, unsigned int n) {
    auto h = (b - a) / T(n);
    T sum1 = 0; T sum2 = 0;
    auto partition = distribute::partition_intervals(n, omp_get_max_threads());
    #pragma omp parallel default(none) shared(sum1, sum2, partition, a, b, h, f) num_threads(partition.threads)
    {
        T local_sum1 = 0, local_sum2 = 0;
        auto part = distribute::omp::partition_get_i(partition);
        auto to = (unsigned)part.i + (unsigned)part.size;
        for (auto k = (unsigned)part.i + 1; k <= to; ++k) {
            auto xk = a + k * h;
            if (k <= partition.intervals - 1)
                local_sum1 += f(xk);
            auto xk_1 = a + (k - 1) * h;
            local_sum2 += f((xk + xk_1) / 2);
        }
        #pragma omp atomic update
        sum1 += local_sum1;
        #pragma omp atomic update
        sum2 += local_sum2;
    }
    T half = T(1) / T(2);
    return h / T(3) * (half * f(a) + sum1 + 2 * sum2 + half * f(b));
}