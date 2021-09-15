#include <iterator>

template <typename T>
class span {
    using len_t = size_t;

    T *const _ptr;
    len_t _length;

  public:
    span(T *ptr, len_t length) : _ptr{ptr}, _length{length} {}
    template<len_t N>
    span(T (&arr)[N]): _ptr{reinterpret_cast<T*const>(std::addressof(arr))}, _length{N} {}

    span sub_span(len_t from, len_t to) const noexcept {
        return span(_ptr + from, to - from);
    }

    auto ptr() noexcept {
        return _ptr;
    }

    auto len() const noexcept {
        return _length;
    }

    auto begin() noexcept {
        return _ptr;
    }

    const auto cbegin() const noexcept {
        return const_cast<const T*>(_ptr);
    }

    auto end() noexcept {
        return _ptr + _length;
    }

    const auto cend() const noexcept {
        return const_cast<const T*>(_ptr + _length);
    }

    T& operator[] (len_t idx) {
        return _ptr[idx];
    }
};