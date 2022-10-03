//
// Created by n3dry on 29.09.22.
//

#pragma once

#include "Iterator.h"
#include "ReverseIterator.h"
#include "Types.h"

namespace TEST {

template<typename T>
class Vector {
public:
    using Iterator = ADS::Iterator<Vector, T>;
    using ReverseIterator = ADS::ReverseIterator<Vector, T>;

    static constexpr size_t s_capacityIncrement = 5;

    Vector() = default;

    ~Vector()
    {
        delete[] m_data;
        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;
    };

    inline size_t size() const
    {
        return m_size;
    }

    inline bool isEmpty() const { return m_size == 0; }

    inline void reserve(size_t newCapacity)
    {
        if (newCapacity == m_capacity) {
            return;
        }

        T* oldData = m_data;
        m_data = new T[newCapacity];
        if (oldData) {
            ADS::memcpy(m_data, oldData, newCapacity * sizeof(T));
            delete[] oldData;
        }
        m_capacity = newCapacity;
    }

    inline T* data()
    {
        return &m_data;
    }

    inline T& at(size_t index)
    {
        static_assert(index > 0 && index < m_size);
        return m_data[index];
    }

    inline T at(size_t index) const
    {
        static_assert(index > 0 && index < m_size);
        return m_data[index];
    }

    inline void pushBack(const T& element)
    {
        if (m_size >= m_capacity) {
            reserve(m_capacity + s_capacityIncrement);
        }
        m_data[m_size++] = element;
    }

    inline void popBack()
    {
        --m_size;
    }

    inline Iterator end()
    {
        return Iterator::end(*this);
    }

    inline Iterator begin()
    {
        return Iterator::begin(*this);
    }

    constexpr inline ReverseIterator rend()
    {
        return ReverseIterator::end(*this);
    }

    constexpr inline ReverseIterator rbegin()
    {
        return ReverseIterator::begin(*this);
    }

    constexpr inline const T& operator[](int index) const
    {
        return m_data[index];
    }

    constexpr inline T& operator[](int index)
    {
        return m_data[index];
    }

private:
    size_t m_size { 0 };
    size_t m_capacity { 0 };
    T* m_data { nullptr };
};

}