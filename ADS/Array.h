//
// Created by n3dry on 29.09.22.
//

#pragma once

#include "Types.h"

namespace ADS {


template<typename T, size_t Size>
struct Array {
    class Iterator {
    public:
        Iterator(Array& array, size_t index)
            : m_array(array)
            , m_index(index)
        {
        }

        Iterator operator++()
        {
            ++m_index;
            return *this;
        }

        Iterator operator++(int)
        {
            ++m_index;
            return *this;
        }

        Iterator operator--()
        {
            --m_index;
            return *this;
        }

        Iterator operator--(int)
        {
            --m_index;
            return *this;
        }

        const T& operator*() const
        {
            return m_array[m_index];
        }

        T& operator*()
        {
            return m_array[m_index];
        }

        bool operator!=(Iterator other) const
        {
            return m_index != other.m_index;
        }

        bool operator==(Iterator other) const
        {
            return m_index == other.m_index;
        }

        bool operator>(Iterator other) const
        {
            return m_index > other.m_index;
        }

        bool operator<(Iterator other) const
        {
            return m_index < other.m_index;
        }

        bool operator>=(Iterator other) const
        {
            return m_index >= other.m_index;
        }

        bool operator<=(Iterator other) const
        {
            return m_index <= other.m_index;
        }

    private:
        Array& m_array;
        size_t m_index { 0 };
    };

    class ReverseIterator {
    public:
        ReverseIterator(Array& array, int index)
            : m_array(array)
            , m_index(index)
        {
        }

        ReverseIterator operator++()
        {
            --m_index;
            return *this;
        }

        ReverseIterator operator++(int)
        {
            --m_index;
            return *this;
        }

        ReverseIterator operator--()
        {
            ++m_index;
            return *this;
        }

        ReverseIterator operator--(int)
        {
            ++m_index;
            return *this;
        }

        const T& operator*() const
        {
            return m_array[m_index];
        }

        T& operator*()
        {
            return m_array[m_index];
        }

        bool operator!=(ReverseIterator other) const
        {
            return m_index != other.m_index;
        }

        bool operator==(ReverseIterator other) const
        {
            return m_index == other.m_index;
        }

        bool operator>(ReverseIterator other) const
        {
            return m_index > other.m_index;
        }

        bool operator<(ReverseIterator other) const
        {
            return m_index < other.m_index;
        }

        bool operator>=(ReverseIterator other) const
        {
            return m_index >= other.m_index;
        }

        bool operator<=(ReverseIterator other) const
        {
            return m_index <= other.m_index;
        }

    private:
        Array& m_array;
        int m_index { 0 };
    };

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

    inline Iterator begin()
    {
        return { *this, 0 };
    }

    inline Iterator end()
    {
        return { *this, size() };
    }

    inline ReverseIterator rend()
    {
        return { *this, -1 };
    }

    inline ReverseIterator rbegin()
    {
        return { *this, static_cast<int>(size()) - 1 };
    }

    inline const T& operator[](size_t index) const
    {
        return m_data[index];
    }

    inline T& operator[](size_t index)
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