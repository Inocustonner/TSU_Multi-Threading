#pragma once
#include <functional>

template <typename T>
T integral_trapezoidal(std::function<T(T)> f, T a, T b, unsigned int intervals) {
    T width = (a + b) / T(intervals);
    T sum = 0;
    T x0 = a, x1 = a + width;
    for (; x1 <= b+T(0.0000001); x0 = x1, x1 += width) {
        T y0 = f(x0), y1 = f(x1);
        sum += (y1 + y0) / 2 * width;
    }
    return sum;
}

// assert a < b
template<typename T>
T integral_recursive(std::function<T(T)> f, T a, T b, T epsilon) {
    T median = (a + b) / 2;
    T sa = f(a) * (median - a), sb = f(b) * (b - median);
    if (std::abs(sb - sa) > epsilon) {
        return integral_recursive(f, a, median, epsilon) + integral_recursive(f, median, b, epsilon);
    }
    return sa + sb;
}