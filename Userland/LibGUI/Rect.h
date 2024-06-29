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

    Rect(const IntPoint& point, Size size)
        : m_position(point)
        , m_size(size)
    {
    }

    Rect(int x, int y, Size size)
        : m_position(x, y)
        , m_size(size)
    {
    }

    inline int width() const { return m_size.width(); }
    inline int height() const { return m_size.height(); }

    void setWidth(int width) { m_size.setWidth(width); }
    void setHeight(int height) { m_size.setHeight(height); }

    inline int x() const { return m_position.x(); }
    inline int y() const { return m_position.y(); }

    void setX(int value) { m_position.setX(value); }
    void setY(int value) { m_position.setY(value); }

    void setSize(Size value) { m_size = value; }
    inline Size size() const { return m_size; }

    void setLeft(int value)
    {
        setX(value);
    }

    void setRight(int value)
    {
        setWidth(value - x() + 1);
    }

    void setTop(int value)
    {
        setY(value);
    }

    void setBottom(int value)
    {
        setHeight(value - y() + 1);
    }

    int left() const { return x(); }
    int right() const { return x() + width(); }
    int top() const { return y(); }
    int bottom() const { return y() + height(); }

    bool isEmpty() const
    {
        return width() == 0 || height() == 0;
    }

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

    bool contains(const Rect& other) const
    {
        return left() <= other.left()
            && right() >= other.right()
            && top() <= other.top()
            && bottom() >= other.bottom();
    }

    void intersect(const Rect& other)
    {
        const int _left = ADS::max(left(), other.left());
        const int _right = ADS::min(right(), other.right());
        const int _top = ADS::max(top(), other.top());
        const int _bottom = ADS::min(bottom(), other.bottom());

        if (_left < _right && _top < _bottom) {
            m_position.setX(_left);
            m_position.setY(_top);
            m_size.setWidth(_right - _left);
            m_size.setHeight(_bottom - _top);
        } else {
            m_position = {};
            m_size = {};
        }
    }

    Rect intersectRect(const Rect& other) const
    {
        Rect copy = *this;
        copy.intersect(other);
        return copy;
    }

    bool intersects(const Rect& other) const
    {
        return ADS::max(left(), other.left()) < ADS::min(right(), other.right())
            && ADS::max(top(), other.top()) < ADS::min(bottom(), other.bottom());
    }

    Rect united(const Rect& other) const;

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

    ADS::String toString() const
    {
        // FIXME: use format
        return ADS::String("[") + std::to_string(x()) + "," + std::to_string(y()) + " " + std::to_string(width()) + "x" + std::to_string(height()) + "]";
    }

private:
    Point<int> m_position {};
    Size m_size {};
};

}