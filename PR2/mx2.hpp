#pragma once
#include <cassert>
#include <utility>

template <typename T>
class mx2 {
    T **_mx = nullptr;
    int _w = 0, _h = 0;

  public:
    mx2() = delete;
    mx2(int w, int h) : _w{w}, _h{h} {
        _mx = new T*[h];
        for (; h >= 0; h--)
            _mx[h] = new T[w]();
    }
    mx2(const mx2 &) = delete;
    mx2(mx2 &&rhs) {
        std::swap(_mx, rhs._mx);
        std::swap(_w, rhs._w);
        std::swap(_h, rhs._h);
    }
    ~mx2() {
        if (_mx != nullptr) {
            for (int h = 0; h < height(); ++h)
                delete[] _mx[h];
        }
    }

    inline 
    bool is_in_w(int w) const noexcept {
        return w >= 0 && w < _w;
    }

    inline 
    bool is_in_h(int h) const noexcept {
        return h >= 0 && h < _h;
    }

    T &operator[](std::initializer_list<int> l) {
        assert(l.size() == 2);
        int wi = *l.begin();
        int hi = *(l.begin() + 1);
        assert(is_in_w(wi) && is_in_h(hi));
        return _mx[hi][wi];
    }

    const T &operator[](std::initializer_list<int> l) const {
        assert(l.size() == 2);
        int wi = *l.begin();
        int hi = *(l.begin() + 1);
        assert(is_in_w(wi) && is_in_h(hi));
        return _mx[hi][wi];
    }

    bool operator==(const mx2& rhs) const noexcept {
        if (rhs._h != _h || rhs._w != _w) {
            return false;
        }
        for (int h = 0; h < _h; ++h)
            for (int w = 0; w < _w; ++w) {
                if (rhs[{w, h}] != (*this)[{w, h}])
                    return false;
            }
        return true;
    }

    auto height() const noexcept {
        return _h;
    }

    auto width() const noexcept {
        return _w;
    }
};