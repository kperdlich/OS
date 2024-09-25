//
// Created by n3dry on 24.09.24.
//

#pragma once

#include "Move.h"
#include <cstdlib>
#include <cstring>
#include <memory>

namespace ADS {

inline void* memcpy(void* dest, const void* src, std::size_t count)
{
    return std::memcpy(dest, src, count);
}

inline void* memset(void* s, int c, size_t n)
{
    return std::memset(s, c, n);
}

inline size_t strlen(const char* ptr)
{
    return std::strlen(ptr);
}

inline void* memmove(void* dest, const void* src, size_t count)
{
    return std::memmove(dest, src, count);
}

inline int memcmp(const void* a, const void* b, size_t size)
{
    return std::memcmp(a, b, size);
}

inline double strtod(const char* str, char** endptr)
{
    return std::strtod(str, endptr);
}

inline long int strtol(const char* str, char** endptr, int base)
{
    return std::strtol(str, endptr, base);
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