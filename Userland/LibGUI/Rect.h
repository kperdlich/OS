//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "Point.h"
#include "Types.h"

namespace GUI {

template<typename T>
class Rect {
public:
    Rect() { }
    Rect(T x, T y, T width, T height)
        : m_position(x, y)
        , m_width(width)
        , m_height(height)
    {
    }

    inline T width() const { return m_width; }
    inline T height() const { return m_height; }

    inline T x() const { return m_position.x(); }
    inline T y() const { return m_position.y(); }

    inline Point<T> position() const { return m_position; }

    inline void moveBy(T x, T y)
    {
        m_position.moveBy(x, y);
    }

    bool contains(T x, T y) const
    {
        if (x > m_position.x() && x < (m_position.x() + m_width) && y > m_position.y() && y < (m_position.y() + m_height))
            return true;
        return false;
    }

    bool contains(const Point<T>& point) const
    {
        if (point.x() > m_position.x() && point.x() < (m_position.x() + m_width) && point.y() > m_position.y() && point.y() < (m_position.y() + m_height))
            return true;
        return false;
    }

    Rect<T> clip(const Rect<T>& clipInside) const
    {
        const T clippedX = ADS::clamp(clipInside.x(), m_position.x(), m_position.x() + m_width);
        const T clippedY = ADS::clamp(clipInside.y(), m_position.y(), m_position.y() + m_height);
        const T clippedWidth = ADS::min(clipInside.m_width - ADS::abs(clippedX - clipInside.x()), m_width - clippedX);
        const T clippedHeight = ADS::min(clipInside.m_height - ADS::abs(clippedY - clipInside.y()), m_height - clippedY);
        return Rect<T> { clippedX, clippedY, clippedWidth, clippedHeight };
    }

private:
    Point<T> m_position {};
    T m_width {};
    T m_height {};
};

using IntRect = Rect<int>;

}