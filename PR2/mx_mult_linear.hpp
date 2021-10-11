#pragma once

#include "mx2.hpp"
#include <exception>

template<typename NumT>
mx2<NumT> mult_linear(const mx2<NumT>& ma, const mx2<NumT>& mb) {
    if (ma.width() != mb.height()) {
        throw std::exception("cannot multiply matrices with these dimensions");
    }
    auto msize = ma.width();
    mx2<NumT> mx(ma.height(), mb.width());
    for (int h = 0; h < ma.height(); ++h) {
        for (int w = 0; w < mb.width(); ++w) {
            for (int m = 0; m < msize; ++m) {
                mx[{w, h}] += ma[{m, h}] * mb[{w, m}];
            }
        }
    }
    return mx;
}