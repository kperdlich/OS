//
// Created by n3dry on 17.09.22.
//

#pragma once

#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>
#include <vector>

#define ASSERT(X) assert(X)

namespace ADS {

using String = std::string;

template<typename T>
using WeakPtr = std::weak_ptr<T>;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename T>
using Vector = std::vector<T>;

inline void* memcpy(void* dest, const void* src, std::size_t count)
{
    return std::memcpy(dest, src, count);
}

inline void* memset(void* s, int c, size_t n)
{
    return std::memset(s, c, n);
}

template<typename T>
T abs(T value)
{
    return std::abs(value);
}

template<typename T>
void swap(T& a, T& b)
{
    std::swap(a, b);
}

template<typename T>
T min(T a, T b)
{
    if (a < b)
        return a;
    return b;
}

template<typename T>
T max(T a, T b)
{
    if (a > b)
        return a;
    return b;
}

template<typename T>
T clamp(T value, T low, T high)
{
    return max(low, min(value, high));
}

}
