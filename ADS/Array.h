//
// Created by n3dry on 29.09.22.
//

#pragma once

namespace ADS {

template<typename T, size_t Size>
struct Array {
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

private:
    T m_data[Size];
};

}