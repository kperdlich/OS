//
// Created by n3dry on 28.05.24.
//

#pragma once

#include "Common.h"
#include "String.h"

namespace GUI {

class Point final {
public:
    Point() = default;
    Point(int x, int y)
        : m_x(x)
        , m_y(y)
    {
    }

    void setX(int x) { m_x = x; }
    void setY(int y) { m_y = y; }

    int x() const { return m_x; }
    int y() const { return m_y; }

    void moveBy(const Point& point)
    {
        m_x += point.x();
        m_y += point.y();
    }

    void moveBy(int x, int y)
    {
        m_x += x;
        m_y += y;
    }

    bool operator==(const Point& other) const
    {
        return m_x == other.m_x
            && m_y == other.m_y;
    }

    Point operator-(const Point& other) const { return { m_x - other.m_x, m_y - other.m_y }; }
    Point operator-() const { return { -m_x, -m_y }; }

    [[nodiscard]] ADS::String toString() const
    {
        ADS::String builder = "[";
        builder.appendInt(m_x);
        builder.append(",");
        builder.appendInt(m_y);
        builder.append("]");
        return builder;
    }

private:
    int m_x {};
    int m_y {};
};

} // GUI
