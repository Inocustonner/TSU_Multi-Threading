#include <cstdio>
#define _USE_MATH_DEFINES
#include <cmath>

#include "trapezoidal_method.hpp"
#include "simpson_method.hpp"

void error_etc(std::function<double(double)> f, unsigned intervals, double x_start, double x_end, double answer) {
    auto trap_ans = integral_trapezoidal_omp<double>(f, x_start, x_end, intervals);
    auto trap_abs_err = trap_ans - answer;
    auto trap_rel_err = trap_abs_err / answer;
    auto simpson_ans = simpson_method_omp<double>(f, x_start, x_end, intervals);
    auto simpson_abs_err = simpson_ans - answer;
    auto simpson_rel_err = simpson_abs_err / answer;
    // printf("sync tp f = %lf\n", integral_trapezoidal_sync<double>(f, x_start, x_end, intervals));
    printf("intervals: %d\n", intervals);
    printf("trapezoidal %.020lf abs(%.020lf) rel(%.020lf)\n", trap_ans, trap_abs_err, trap_rel_err);
    // printf("sync sm f = %lf\n", simpson_method_sync<double>(f, x_start, x_end, intervals));
    printf("simpson %.020lf abs(%.020lf) rel(%.020lf)\n", simpson_ans, simpson_abs_err, simpson_rel_err);
}

void error_etc_default_intervals(std::function<double(double)> f, double x_start, double x_end, double answer) {
    error_etc(f, 5, x_start, x_end, answer);
    printf("---\n");
    error_etc(f, 50, x_start, x_end, answer);
    printf("---\n");
    error_etc(f, 100, x_start, x_end, answer);
    printf("---\n");
    error_etc(f, 500, x_start, x_end, answer);
    printf("---\n");
    error_etc(f, 1000, x_start, x_end, answer);
}

void t1() {
    printf("t1\n");
    auto f = [](double x) { return x * x * x - log(x) + 1; };
    double x0 = 1, x1 = 2;
    // auto invs = 100u;
    error_etc_default_intervals(f, x0, x1, 4.36370563888);
}

void t2() {
    printf("t2\n");
    printf("method 1\n---|\n");
    auto f_pi_1 = [](double x) { return 4 * sqrt(1 - x * x); };
    error_etc_default_intervals(f_pi_1, 0, 1, M_PI);
    putchar('\n');
    printf("method 2\n---|\n");
    auto f_pi_2 = [](double x) { return 4 / (1 + x * x); };
    error_etc_default_intervals(f_pi_2, 0, 1, M_PI);
}

int main() {
    t1();
    putchar('\n');
    putchar('\n');
    t2();
}