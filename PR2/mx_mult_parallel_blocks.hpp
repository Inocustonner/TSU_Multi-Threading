#pragma once

#include "mx2.hpp"
#include <exception>
#include <omp.h>

template <typename NumT, int blocks_num=2>
mx2<NumT> mult_parallel_blocks(const mx2<NumT> &ma, const mx2<NumT> &mb) {
    if (ma.width() != mb.height()) {
        throw std::exception("cannot multiply matrices with these dimensions");
    }
    if (ma.width() != ma.height()) {
        throw std::exception("can only multiply square matrices");
    }
    auto msize = ma.width();
    mx2<NumT> mx(ma.height(), mb.width());
    auto block_size = msize / blocks_num;
    omp_set_num_threads(blocks_num * blocks_num);
    #pragma omp parallel default(none) shared(ma, mb, msize, mx, block_size)
    {
        int thread_id = omp_get_thread_num();
        int row_idx = thread_id / block_size;
        int coll_idx = thread_id % block_size;

        int h = row_idx * block_size;
        int h_limit = h + block_size;
        for (; h < h_limit; ++h) {
            int w = coll_idx * block_size;
            int w_limit = w + block_size;
            for (; w < w_limit; ++w) {
                for (int m = 0; m < msize; ++m) {
                    mx[{w, h}] += ma[{m, h}] * mb[{w, m}];
                }
            }
        }
    }
    return mx;
}