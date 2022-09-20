//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "Types.h"

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

    inline T width() const { return m_width; }
    inline T height() const { return m_height; }

    inline T x() const { return m_x; }
    inline T y() const { return m_y; }

    inline void moveBy(T x, T y)
    {
        m_x += x;
        m_y += y;
    }

    bool isPointInside(T x, T y) const
    {
        if (x > m_x && x < (m_x + m_width) && y > m_y && y < (m_y + m_height))
            return true;
        return false;
    }

    Rect<T> clip(Rect<T> clipInside) const
    {
        const T clippedX = ADS::clamp(clipInside.m_x, m_x, m_x + m_width);
        const T clippedY = ADS::clamp(clipInside.m_y, m_y, m_y + m_height);
        const T clippedWidth = ADS::min(clipInside.m_width - ADS::abs(clippedX - clipInside.m_x), m_width - clippedX);
        const T clippedHeight = ADS::min(clipInside.m_height - ADS::abs(clippedY - clipInside.m_y), m_height - clippedY);
        return Rect<T> { clippedX, clippedY, clippedWidth, clippedHeight };
    }

private:
    T m_x, m_y;
    T m_width, m_height;
};

using IntRect = Rect<int>;

}