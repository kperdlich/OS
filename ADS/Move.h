//
// Created by n3dry on 30.07.24.
//

#pragma once

namespace ADS {

template <typename T>
T&& move(T& arg)
{
    return static_cast<T&&>(arg);
}

template<typename T>
struct RemoveReference {
    typedef T Type;
};

template<typename T>
T&& forward(typename RemoveReference<T>::Type& param)
{
    return static_cast<T&&>(param);
}

}
