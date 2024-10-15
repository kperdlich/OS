//
// Created by n3dry on 17.09.22.
//

#pragma once

#include <stdint.h>

namespace ADS {

typedef decltype(sizeof(0)) size_t; // valid since C++11
typedef signed int ssize_t;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

}
