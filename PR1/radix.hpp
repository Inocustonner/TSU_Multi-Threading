#pragma once
#include <helpers/span.hpp>
#include <type_traits>
#include <vector>

template<typename T, int bits>
constexpr T mask_bits = ~((~T(0) >> bits) << bits); // first <bits> bits will be 1

template<typename T>
constexpr int bits_in = sizeof(T) * 8;

template<int n>
constexpr bool is_power_of_2_v = (n & (n - 1)) == 0 && n != 0;

template<typename T, int bits = 2>
void radix_msd(span<T> arr, int offset = 0) {
    static_assert(std::is_integral_v<T> && is_power_of_2_v<bits>, "");
    if (offset >= bits_in<T>)
        return;
    std::vector<T> buckets[1 << bits];
    auto inv_offset = bits_in<T> - bits - offset;
    // make mask for current bits
    auto mask = static_cast<T>(mask_bits<std::make_unsigned_t<T>, bits> << inv_offset);

    for (T v: arr) {
        buckets[(v & mask) >> inv_offset].push_back(v);
    }

    int i = 0;
    for (auto& bucket: buckets) {
        if (bucket.size() > 1)
            radix_msd<T, bits>(span<T>(bucket.data(), bucket.size()), offset + bits);
        for (auto v: bucket) arr[i++] = v;
    }
}