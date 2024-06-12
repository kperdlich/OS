//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "Point.h"
#include "Size.h"
#include "Types.h"

namespace GUI {

class Rect {
public:
    Rect() = default;
    Rect(int x, int y, int width, int height)
        : m_position(x, y)
        , m_size(width, height)
    {
    }

    inline int width() const { return m_size.width(); }
    inline int height() const { return m_size.height(); }

    void setWidth(int width) { m_size.setWidth(width); }
    void setHeight(int height) { m_size.setHeight(height); }

    inline int x() const { return m_position.x(); }
    inline int y() const { return m_position.y(); }

    inline Size size() const { return m_size; }

    inline Point<int> position() const { return m_position; }

    inline void moveBy(int x, int y)
    {
        m_position.moveBy(x, y);
    }

    void moveBy(const Point<int>& point)
    {
        m_position.moveBy(point);
    }

    bool contains(int x, int y) const
    {
        return x >= m_position.x() && x < (m_position.x() + width()) && y >= m_position.y() && y < (m_position.y() + height());
    }

    bool contains(const IntPoint& point) const
    {
        return contains(point.x(), point.y());
    }

    Rect clip(const Rect& clipInside) const
    {
        const int clippedX = ADS::clamp(clipInside.x(), m_position.x(), m_position.x() + m_size.width());
        const int clippedY = ADS::clamp(clipInside.y(), m_position.y(), m_position.y() + m_size.height());
        const int clippedWidth = ADS::min(clipInside.width() - ADS::abs(clippedX - clipInside.x()), m_size.width() - clippedX);
        const int clippedHeight = ADS::min(clipInside.height() - ADS::abs(clippedY - clipInside.y()), m_size.height() - clippedY);
        return Rect { clippedX, clippedY, clippedWidth, clippedHeight };
    }

    bool operator==(const Rect& other) const
    {
        return m_position == other.m_position
            && m_size == other.m_size;
    }

private:
    Point<int> m_position {};
    Size m_size {};
};

}