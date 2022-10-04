//
// Created by n3dry on 29.09.22.
//

#pragma once

#include "Iterator.h"
#include "ReverseIterator.h"

namespace ADS {

template<typename T, size_t Size>
struct Array {
    using Iterator = ADS::Iterator<Array, T>;
    using ReverseIterator = ADS::ReverseIterator<Array, T>;

    inline T* data()
    {
        return &m_data;
    }

    inline T data() const
    {
        return m_data;
    }

    T& first() requires(Size > 0)
    {
        return m_data[0];
    }

    inline T& at(size_t index)
    {
        static_assert(index > 0 && index < Size);
        return m_data[index];
    }

    inline T at(size_t index) const
    {
        static_assert(index > 0 && index < Size);
        return m_data[index];
    }

    inline Iterator end()
    {
        return Iterator::end(*this);
    }

    inline Iterator begin()
    {
        return Iterator::begin(*this);
    }

    inline ReverseIterator rend()
    {
        return ReverseIterator::end(*this);
    }

    inline ReverseIterator rbegin()
    {
        return ReverseIterator::begin(*this);
    }

    inline const T& operator[](int index) const
    {
        return m_data[index];
    }

    inline T& operator[](int index)
    {
        return m_data[index];
    }

    inline size_t size()
    {
        return Size;
    }

    T m_data[Size];
};
}