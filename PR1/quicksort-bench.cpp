#include <cstdlib>
#include <algorithm>
#include <helpers/helpers.hpp>
#include <helpers/span.hpp>
#include <omp.h>

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
void quick_sort_sync(span<T> arr) {
    if (arr.len() > sort_thr) {
        std::nth_element(arr.begin(), arr.begin() + arr.len() / 2, arr.end());
        auto p = arr.len() / 2;
        quick_sort_sync(arr.sub_span(0, p));
        quick_sort_sync(arr.sub_span(p, arr.len()));
    } else {
        shake_sort(arr);
    }
}
// thread will calculate region for sorting and sort it
template <typename T, int sort_thr = 16>
void quick_sort_multi_threaded_thread(span<T> arr) {
    if (arr.len() > sort_thr) {
        std::nth_element(arr.begin(), arr.begin() + arr.len() / 2, arr.end());
        auto p = arr.len() / 2;
        #pragma omp task
        quick_sort_multi_threaded_thread<T, sort_thr>(arr.sub_span(0, p));
        #pragma omp task
        quick_sort_multi_threaded_thread<T, sort_thr>(arr.sub_span(p, arr.len()));
        #pragma omp taskwait
    } else {
        shake_sort(arr);
    }
}

template <typename T, int sort_thr = 16>
void quick_sort_multi_threaded(span<T> arr) {
    #pragma omp parallel default(none) shared(arr)
    {
        #pragma omp single nowait
        quick_sort_multi_threaded_thread<T, sort_thr>(arr);
    }
    // printf("%6d %8s\n", arr.len(), std::is_sorted(arr.cbegin(), arr.cend()) ? "sorted": "not sorted");
}

void populate(span<int> arr) {
    for (int& v: arr) {
        v = std::rand();
    }
}

void quick_sort_test_mp() {
    constexpr int bench_iterations = 8;
    constexpr size_t max_data_size = 16384 * 4;
    int sizes[] = {32, 128, 256, 512, 1024, 2048, 4096, 8000, 16384, 16384 * 2,  max_data_size};
    int data[max_data_size];

    printf("iterations %d\n", bench_iterations);
    for (auto size: sizes) {
        auto arr = span<int>(reinterpret_cast<int*>(data), size);
        auto info_st = benchmark([arr](){quick_sort_sync(arr);}, [arr](){populate(arr);}, bench_iterations);
        auto info_mt = benchmark([arr]() {quick_sort_multi_threaded(arr); }, [arr](){populate(arr);}, bench_iterations);


        // for (int i = 1; i < arr.len(); ++i) {
        //     if (arr[i - 1] > arr[i])
        //         printf("%d > %d", arr[i - 1], arr[i]);
        // }
        // putchar('\n');
        // for (auto v: arr)
        //     printf("%d ", v);
        // putchar('\n');
        printf("%11.4f \t%9.3f ms \t| elements %5d\n",
            info_st.average, 
            info_mt.average,
            size);
    }
}

void quicksort_bench() {
    std::srand(std::time(nullptr));
    quick_sort_test_mp();
}