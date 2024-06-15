//
// Created by n3dry on 19.09.22.
//

#pragma once

namespace GUI {

class Size {
public:
    Size() = default;
    Size(int width, int height)
        : m_width(width)
        , m_height(height)
    {
    }

    inline int width() const { return m_width; }
    inline int height() const { return m_height; }

    inline void setWidth(int value) { m_width = value; }
    inline void setHeight(int value) { m_height = value; }

    bool operator==(const Size& other) const
    {
        return m_width == other.m_width
            && m_height == other.m_height;
    }

    Size& operator-=(const Size& other)
    {
        m_height -= other.m_height;
        m_width -= other.m_width;
        return *this;
    }

private:
    int m_width {};
    int m_height {};
};

} // GUI
