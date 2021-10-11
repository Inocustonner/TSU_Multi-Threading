#pragma once

#include "mx2.hpp"
#include <exception>
#include <omp.h>

template <typename NumT>
mx2<NumT> mult_parallel_tasks(const mx2<NumT> &ma, const mx2<NumT> &mb) {
    if (ma.width() != mb.height()) {
        throw std::exception("cannot multiply matrices with these dimensions");
    }
    auto msize = ma.width();
    mx2<NumT> mx(ma.height(), mb.width());
    #pragma omp parallel default(none)shared(ma, mb, msize, mx)
    #pragma omp single
    {
        for (int h = 0; h < ma.height(); ++h){
            for (int w = 0; w < mb.width(); ++w) 
            #pragma omp task default(none) firstprivate(h, w) shared(ma, mb, msize, mx)
            {
                for (int m = 0; m < msize; ++m) {
                    mx[{w, h}] += ma[{m, h}] * mb[{w, m}];
                }
            }
        }
        #pragma omp taskwait
    }
    return mx;
}