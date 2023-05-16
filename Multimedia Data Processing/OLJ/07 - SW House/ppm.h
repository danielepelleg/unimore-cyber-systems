#pragma once

#include <string>
#include <cstdint>
#include <array>

#include "mat.h"

template <typename T, size_t N>
struct vec
{
    std::array<T, N> data_;

    template <typename... Args>
    vec(Args... args) : data_{ { args... } } {}

    const T& operator[](size_t n) const { return data_[n]; }
    T& operator[](size_t n) { return data_[n]; }
};

using vec3b = vec<uint8_t, 3>;

std::istream& operator>>(std::istream& is, vec3b& v);