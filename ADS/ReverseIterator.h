//
// Created by n3dry on 03.10.22.
//

#pragma once

namespace ADS {

template<typename Container, typename Value>
class ReverseIterator {
public:
    ReverseIterator(Container& container, int index)
        : m_container(container)
        , m_index(index)
    {
    }

    ReverseIterator operator++()
    {
        --m_index;
        return *this;
    }

    ReverseIterator operator++(int index)
    {
        m_index -= index;
        return *this;
    }

    ReverseIterator operator--()
    {
        ++m_index;
        return *this;
    }

    ReverseIterator operator--(int index)
    {
        m_index += index;
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

    static ReverseIterator begin(Container& container)
    {
        return { container, static_cast<int>(container.size()) - 1 };
    }

    static ReverseIterator end(Container& container)
    {
        return { container, -1 };
    }

private:
    friend Container;

    Container& m_container;
    int m_index { 0 };
};

}