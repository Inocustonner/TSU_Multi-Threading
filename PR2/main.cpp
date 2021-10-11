#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <unordered_map>

#include <helpers/colors.hpp>

#include "mx2.hpp"
#include "mx_mult_linear.hpp"
#include "mx_mult_parallel_tasks.hpp"
#include "mx_mult_parallel_blocks.hpp"

void print_omp_vesion() {
    std::unordered_map<unsigned, const char *> map{
        {200505, "2.5"}, {200805, "3.0"}, {201107, "3.1"}, {201307, "4.0"}, {201511, "4.5"}, {201811, "5.0"}, {202011, "5.1"}};
    printf("OMP version          : %s\n", map[_OPENMP]);
    printf("Max threads          : %3d\n", omp_get_max_threads());
    printf("Number of processors : %3d\n\n", omp_get_num_procs());
}

constexpr int max_int = 100;

void fill_mx(mx2<int>& mx) {
    for (int h = 0; h < mx.height(); ++h) {
        for (int w = 0; w < mx.width(); ++w) {
            mx[{w, h}] = std::abs(std::rand()) % max_int;
        }
    }
}

void print_mx(const mx2<int>& mx) {
    for (int h = 0; h < mx.height(); ++h) {
        putchar('|');
        for (int w = 0; w < mx.width(); ++w) {
            printf(" %5d |", mx[{w, h}]);
        }
        putchar('\n');
    }
}

void print_and_check(const mx2<int>& ma, const mx2<int>& mb, const mx2<int>& mc) {
    print_mx(mc);
    auto mcheck = mult_linear(ma, mb);
    printf("Check: %s\n", mc == mcheck ? BOLD(FGRN("valid")) : BOLD(FRED("invalid")));
}

int main() {
    std::srand(std::time(nullptr));
    print_omp_vesion();

    int w = 4, h = 4;
    mx2<int> ma(w, h);
    mx2<int> mb(w, h);

    fill_mx(ma);
    fill_mx(mb);

    printf("ma\n");
    print_mx(ma);
    printf("mb\n");
    print_mx(mb);
    printf("ma x mc\n");
    print_and_check(ma, mb, mult_parallel_tasks(ma, mb));
    printf("tasks\n");
    print_and_check(ma, mb, mult_parallel_blocks(ma, mb));
    printf("blocks\n");
}