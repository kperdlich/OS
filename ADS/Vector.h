//
// Created by n3dry on 29.09.22.
//

#pragma once

#include "Types.h"
#include "initializer_list"

namespace TEST {

template<typename T>
class Vector {
public:
    class Iterator {
    public:
        Iterator(Vector& vector, size_t index)
            : m_vector(vector)
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
            return m_vector[m_index];
        }

        T& operator*()
        {
            return m_vector[m_index];
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
        Vector& m_vector;
        size_t m_index { 0 };
    };

    class ReverseIterator {
    public:
        ReverseIterator(Vector& vector, int index)
            : m_vector(vector)
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
            return m_vector[m_index];
        }

        T& operator*()
        {
            return m_vector[m_index];
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
        Vector& m_vector;
        int m_index { 0 };
    };

public:
    static constexpr size_t s_capacityIncrement = 5;

    Vector() = default;

    Vector(std::initializer_list<T> list)
    {
        for (auto& it : list) {
            pushBack(it);
        }
    };

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

    inline const T& operator[](int index) const
    {
        return m_data[index];
    }

    inline T& operator[](int index)
    {
        return m_data[index];
    }

private:
    size_t m_size { 0 };
    size_t m_capacity { 0 };
    T* m_data { nullptr };
};

}