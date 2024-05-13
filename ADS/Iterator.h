//
// Created by n3dry on 03.10.22.
//

#pragma once

#include "Types.h"

namespace ADS {

template<typename Container, typename Value>
class Iterator {
public:
    Iterator(Container& container, size_t index)
        : m_container(container)
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

    const Value& operator*() const
    {
        return m_container[m_index];
    }

    Value& operator*()
    {
        return m_container[m_index];
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

    static Iterator begin(Container& container)
    {
        return { container, 0 };
    }

    static Iterator end(Container& container)
    {
        return { container, static_cast<size_t>(container.size()) };
    }

private:
    Container& m_container;
    size_t m_index { 0 };
};

}