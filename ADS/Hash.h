//
// Created by n3dry on 09.08.24.
//

#pragma once

#include "HashFunctions.h"
#include "Types.h"

namespace ADS {

template<typename T>
struct Hash {
};

template<>
struct Hash<uint32> {
    uint32 operator()(uint32 value) const
    {
        return hashInt(value);
    }
};

template<>
struct Hash<int32> {
    uint32 operator()(int32 value) const
    {
        return hashInt(value);
    }
};

template<typename T>
struct Hash<T*> {
    uint32 operator()(const T* ptr) const
    {
        return hashInt(static_cast<uint32>(ptr));
    }
};

}
