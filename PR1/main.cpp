#include "quicksort.hpp"
#include "integral_calc.hpp"
#include "radix.hpp"
#include <chrono>
#include <helpers/choice.hpp>
#include <helpers/helpers.hpp>

extern void quicksort_bench();       // quicksort-bench.cpp
extern void populate(span<int> arr); // quicksort-bench.cpp

void quicksort_task() {
    int n = readv<int>("enter number of elements: ");
    int *input = new int[n];

    auto arr = span<int>(input, n);
    printf("populating with random elements\n");
    populate(arr);
    printf("sorting\n");
    auto bench = benchmark([arr]() { quicksort(arr); });

    printf("quicksort | %d elements | %f ms | %s\n\n",
           n,
           bench.average,
           std::is_sorted(arr.cbegin(), arr.cend()) ? "sorted" : "not sorted");

    delete[] input;
}

void integral_task_trapezoidal() {
    printf("\x9F(x) = sin(x)*x^2, with x, y: float\n");
    auto a = readv<double>("enter start: ");
    auto b = readv<double>("enter end: ");
    auto intervals = readv<unsigned int>("enter number of intervals: ");
    auto f = [](double x) -> double { return sin(x) * x * x; };
    printf("%6.2f\n   \xDA\n   \xB3\x9F = %f\n   \xD9\n%6.2f\n\n", b, integral_trapezoidal<double>(f, a, b, intervals), a);
}

void integral_task_recursive() {
    printf("\x9F(x) = sin(x)*x^2, with x, y: float\n");
    auto a = readv<double>("enter start: ");
    auto b = readv<double>("enter end: ");
    auto epsilon = readv<double>("enter epsilon: ");
    auto f = [](double x) -> double { return sin(x) * x * x; };
    printf("%6.2f\n   \xDA\n   \xB3\x9F = %f\n   \xD9\n%6.2f\n\n", b, integral_recursive<double>(f, a, b, epsilon), a);
}

// kinda bad, but it ok in this case
int main() {
    // std::srand(std::time(nullptr));
    // bool run = true;
    // choice choices{};
    // choices
    //     .add_branch("run quicksort", quicksort_task)
    //     .add_branch("run integral calculation(trapezoidal)", integral_task_trapezoidal)
    //     .add_branch("run integral calculation(recursive)", integral_task_recursive)
    //     .add_branch("bench quicksort st vs mt", quicksort_bench)
    //     .add_branch("stop", [&run]() { run = false; });

    // while (run)
    //     choices.branch("Choose what to do"); 

    int test[16000];
    span<int> arr(test);
    populate(arr);
    radix_msd(arr);
    printf("%s", std::is_sorted(arr.cbegin(), arr.cend()) ? "sorted": "not sorted");
    // for (auto v: arr) printf("%d ", v);
}