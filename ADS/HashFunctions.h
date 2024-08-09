//
// Created by n3dry on 09.08.24.
//

#pragma once

#include "Types.h"

namespace ADS {

inline uint32 hashInt(uint32 value)
{
    value ^= value >> 16;
    value *= 0x85ebca6b;
    value ^= value >> 13;
    value *= 0xc2b2ae35;
    value ^= value >> 16;
    return value;
}

}
