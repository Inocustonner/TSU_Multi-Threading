#pragma once
#include <helpers/span.hpp>
#include <algorithm>

template <typename T>
void shake_sort(span<T> arr) {
    using idx_t = decltype(arr.len());
    idx_t min_i = 0, top_i = arr.len() - 1;

    while (min_i < top_i) {
        idx_t new_top_i = min_i;
        for (idx_t i = min_i; i < top_i; ++i) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                new_top_i = i;
            }
        }
        top_i = new_top_i;

        idx_t new_min_i = top_i;
        for (idx_t i = top_i; i > min_i; --i) {
            if (arr[i] < arr[i - 1]) {
                std::swap(arr[i], arr[i - 1]);
                new_min_i = i;
            }
        }
        min_i = new_min_i;
    }
}

template <typename T, int sort_thr = 16>
void quicksort(span<T> arr) {
    if (arr.len() > sort_thr) {
        std::nth_element(arr.begin(), arr.begin() + arr.len() / 2, arr.end());
        auto p = arr.len() / 2;
        quicksort(arr.sub_span(0, p));
        quicksort(arr.sub_span(p, arr.len()));
    } else {
        shake_sort(arr);
    }
}