//
// Created by n3dry on 19.09.22.
//

#pragma once

namespace GUI {

class Size {
public:
    static Size Invalid() { return { -1, -1 }; }

    Size() = default;
    Size(int width, int height)
        : m_width(width)
        , m_height(height)
    {
    }

    bool isInvalid() const { return m_width <= 0 || m_height <= 0; }

    inline int width() const { return m_width; }
    inline int height() const { return m_height; }

    inline void setWidth(int value) { m_width = value; }
    inline void setHeight(int value) { m_height = value; }

    bool operator==(const Size& other) const
    {
        return m_width == other.m_width
            && m_height == other.m_height;
    }

    bool operator!=(const Size& other) const
    {
        return !(*this == other);
    }

    Size& operator-=(const Size& other)
    {
        m_height -= other.m_height;
        m_width -= other.m_width;
        return *this;
    }

    Size& operator+=(const Size& other)
    {
        m_height += other.m_height;
        m_width += other.m_width;
        return *this;
    }

private:
    int m_width {};
    int m_height {};
};

} // GUI
