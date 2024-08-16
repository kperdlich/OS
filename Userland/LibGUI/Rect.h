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

    Rect(const Point& point, Size size)
        : m_position(point)
        , m_size(size)
    {
    }

    Rect(int x, int y, Size size)
        : m_position(x, y)
        , m_size(size)
    {
    }

    int width() const { return m_size.width(); }
    int height() const { return m_size.height(); }

    void setWidth(int width) { m_size.setWidth(width); }
    void setHeight(int height) { m_size.setHeight(height); }

    inline int x() const { return m_position.x(); }
    inline int y() const { return m_position.y(); }

    void setX(int value) { m_position.setX(value); }
    void setY(int value) { m_position.setY(value); }

    void setSize(Size value) { m_size = value; }
    inline Size size() const { return m_size; }

    void setPosition(const Point& position)
    {
        m_position = position;
    }

    void setLeft(int value)
    {
        setX(value);
    }

    void setRight(int value)
    {
        setWidth(value - x());
    }

    void setTop(int value)
    {
        setY(value);
    }

    void setBottom(int value)
    {
        setHeight(value - y());
    }

    int left() const { return x(); }
    int right() const { return x() + width(); }
    int top() const { return y(); }
    int bottom() const { return y() + height(); }

    bool isEmpty() const
    {
        return width() <= 0 || height() <= 0;
    }

    inline Point position() const { return m_position; }

    inline void moveBy(int x, int y)
    {
        m_position.moveBy(x, y);
    }

    void moveBy(const Point& point)
    {
        m_position.moveBy(point);
    }

    [[nodiscard]] Point center() const
    {
        return { x() + width() / 2, y() + height() / 2 };
    }

    bool contains(int x, int y) const
    {
        return x >= left() && x < right() && y >= top() && y < bottom();
    }

    bool contains(const Point& point) const
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

        if (_left > _right || _top > _bottom) {
            m_position = {};
            m_size = {};
            return;
        }

        m_position.setX(_left);
        m_position.setY(_top);
        m_size.setWidth(_right - _left);
        m_size.setHeight(_bottom - _top);
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

    bool operator==(const Rect& other) const
    {
        return m_position == other.m_position
            && m_size == other.m_size;
    }

    ADS::String toString() const
    {
        // FIXME: use format
        ADS::String builder;
        builder.append("[");
        builder.appendInt(x());
        builder.append(",");
        builder.appendInt(y());
        builder.append(" ");
        builder.appendInt(width());
        builder.append("/");
        builder.appendInt(height());
        builder.append("]");
        return builder;
    }

private:
    Point m_position {};
    Size m_size {};
};

}