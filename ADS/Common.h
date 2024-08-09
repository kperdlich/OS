//
// Created by n3dry on 09.08.24.
//

#pragma once

#include "Assert.h"
#include "Move.h"
#include "OwnPtr.h"
#include "RefPtr.h"
#include "Types.h"
#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <thread>
#include <unordered_map>
#include <vector>

namespace ADS {

using String = std::string;

template<typename T>
using Function = std::function<T>;

template<typename T>
using Optional = std::optional<T>;

template<typename T>
using Vector = std::vector<T>;

template<typename T>
using LockGuard = std::lock_guard<T>;

using Mutex = std::mutex;

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