//
// Created by n3dry on 29.09.22.
//

#pragma once

#include "Assert.h"
#include "LibC.h"
#include "Types.h"
#include "initializer_list"

namespace ADS {

template<typename T>
class Vector {
public:
    class Iterator {
        friend class Vector;

    public:
        Iterator(Vector& vector, ADS::size_t index)
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
        ADS::size_t m_index { 0 };
    };

    class ConstIterator {
    public:
        ConstIterator(const Vector& vector, ADS::size_t index)
            : m_vector(vector)
            , m_index(index)
        {
        }

        ConstIterator operator++()
        {
            ++m_index;
            return *this;
        }

        ConstIterator operator++(int)
        {
            ++m_index;
            return *this;
        }

        ConstIterator operator--()
        {
            --m_index;
            return *this;
        }

        ConstIterator operator--(int)
        {
            --m_index;
            return *this;
        }

        const T& operator*() const
        {
            return m_vector[m_index];
        }

        bool operator!=(ConstIterator other) const
        {
            return m_index != other.m_index;
        }

        bool operator==(ConstIterator other) const
        {
            return m_index == other.m_index;
        }

        bool operator>(ConstIterator other) const
        {
            return m_index > other.m_index;
        }

        bool operator<(ConstIterator other) const
        {
            return m_index < other.m_index;
        }

        bool operator>=(ConstIterator other) const
        {
            return m_index >= other.m_index;
        }

        bool operator<=(ConstIterator other) const
        {
            return m_index <= other.m_index;
        }

    private:
        const Vector& m_vector;
        ADS::size_t m_index { 0 };
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
    static constexpr ADS::size_t s_capacityIncrement = 5;

    Vector() = default;

    Vector(std::initializer_list<T> list)
    {
        for (auto& it : list) {
            pushBack(it);
        }
    };

    Vector(const Vector& other) = delete;

    Vector(Vector&& other)
        : m_size(other.m_size)
        , m_data(other.m_data)
        , m_capacity(other.m_capacity)
    {

        other.m_size = 0;
        other.m_capacity = 0;
        other.m_data = nullptr;
    };

    ~Vector()
    {
        clear();
    };

    Vector& operator=(Vector& other) = delete;

    Vector& operator=(Vector&& other)
    {
        if (this == &other)
            return *this;

        clear();

        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        other.m_size = 0;
        other.m_capacity = 0;
        other.m_data = nullptr;
    }

    [[nodiscard]] ADS::size_t size() const
    {
        return m_size;
    }

    bool isEmpty() const { return m_size == 0; }

    void reserve(ADS::size_t newCapacity)
    {
        if (newCapacity <= m_capacity)
            return;

        T* oldData = m_data;
        m_data = static_cast<T*>(ADS::malloc(newCapacity * sizeof(T)));
        if (oldData) {
            for (ADS::size_t i = 0; i < m_size; ++i)
                new (&m_data[i]) T(ADS::move(oldData[i]));
            ADS::free(oldData);
            oldData = nullptr;
        }
        m_capacity = newCapacity;
    }

    T* data()
    {
        return &m_data;
    }

    T& at(ADS::size_t index)
    {
        ASSERT(index >= 0 && index < m_size);
        return m_data[index];
    }

    T at(ADS::size_t index) const
    {
        ASSERT(index >= 0 && index < m_size);
        return m_data[index];
    }

    void pushBack(T&& element)
    {
        if (m_size >= m_capacity) {
            reserve(m_capacity + s_capacityIncrement);
        }
        new (&m_data[m_size++]) T(ADS::move(element));
    }

    void pushBack(const T& element)
    {
        if (m_size >= m_capacity) {
            reserve(m_capacity + s_capacityIncrement);
        }
        new (&m_data[m_size++]) T(element);
    }

    void popBack()
    {
        if (isEmpty())
            return;

        m_data[--m_size].~T();
    }

    ADS::size_t removeAll(const T& valueToRemove)
    {
        size_t removedElements = 0;
        for (ADS::size_t i = 0; i < m_size;) {
            if (m_data[i] == valueToRemove) {
                remove(Iterator(*this, i));
                ++removedElements;
            } else {
                ++i;
            }
        }
        return removedElements;
    }

    void remove(const Iterator& pos)
    {
        if (isEmpty() || pos.m_index > m_size || pos.m_index < 0)
            return;

        m_data[pos.m_index].~T();

        // Shift everything after the iterator to the left.
        for (ADS::size_t i = pos.m_index + 1; i < m_size; ++i) {
            m_data[i - 1] = ADS::move(m_data[i]);
        }
        --m_size;
    }

    Iterator find(const T& value)
    {
        for (ADS::size_t i = 0; i < m_size; ++i) {
            if (value == m_data[i])
                return Iterator(*this, i);
        }

        return end();
    }

    void resize(ADS::size_t newSize)
    {
        if (newSize == m_size)
            return;

        // Increase size and append default values
        if (newSize > m_size) {
            if (m_capacity < newSize)
                reserve(newSize + s_capacityIncrement);
            for (ADS::size_t i = m_size; i < newSize; ++i)
                m_data[i] = {};
            m_size = newSize;
            return;
        }

        // Reduce size
        if (newSize < m_size) {
            for (ADS::size_t i = newSize; i < m_size; ++i)
                m_data[i].~T();
            m_size = newSize;
            return;
        }
    }

    void clear()
    {
        for (ADS::size_t i = 0; i < m_size; ++i)
            m_data[i].~T();

        ADS::free(m_data);
        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;
    }

    Iterator begin()
    {
        return { *this, 0 };
    }

    Iterator end()
    {
        return { *this, size() };
    }

    ConstIterator begin() const
    {
        return { *this, 0 };
    }

    ConstIterator end() const
    {
        return { *this, size() };
    }

    ReverseIterator rend()
    {
        return { *this, -1 };
    }

    ReverseIterator rbegin()
    {
        return { *this, static_cast<int>(size()) - 1 };
    }

    const T& operator[](int index) const
    {
        return m_data[index];
    }

    T& operator[](int index)
    {
        return m_data[index];
    }

private:
    ADS::size_t m_size { 0 };
    ADS::size_t m_capacity { 0 };
    T* m_data { nullptr };
};

}