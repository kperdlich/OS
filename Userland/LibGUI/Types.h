//
// Created by n3dry on 17.09.22.
//

#pragma once

#include <cassert>
#include <cstring>
#include <iostream>

#define ASSERT(X) assert(X)

namespace ADS {
using String = std::string;

inline void* memcpy(void* dest, const void* src, std::size_t count)
{
    return std::memcpy(dest, src, count);
}

inline void* memset(void* s, int c, size_t n)
{
    return std::memset(s, c, n);
}
}
