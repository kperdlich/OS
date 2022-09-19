//
// Created by n3dry on 17.09.22.
//

#pragma once

namespace GUI {

template<typename T>
class Rect {
public:
    Rect(T x, T y, T width, T height)
        : m_x(x)
        , m_y(y)
        , m_width(width)
        , m_height(height)
    {
    }

    inline T width() { return m_width; }
    inline T height() { return m_height; }

    inline T x() { return m_x; }
    inline T y() { return m_y; }

    inline void moveBy(T x, T y)
    {
        m_x += x;
        m_y += y;
    }

    bool isPointInside(T x, T y)
    {
        if (x > m_x && x < (m_x + m_width) && y > m_y && y < (m_y + m_height))
            return true;
        return false;
    }

private:
    T m_x, m_y;
    T m_width, m_height;
};

using IntRect = Rect<int>;

}