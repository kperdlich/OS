//
// Created by n3dry on 28.05.24.
//

#pragma once

namespace GUI {

template<typename T>
class Point final {
public:
    Point() = default;
    Point(T x, T y)
        : m_x(x)
        , m_y(y)
    {
    }

    void setX(T x) { m_x = x; }
    void setY(T y) { m_y = y; }

    T x() const { return m_x; }
    T y() const { return m_y; }

    void moveBy(const Point<T>& point)
    {
        m_x += point.x();
        m_y += point.y();
    }

    void moveBy(T x, T y)
    {
        m_x += x;
        m_y += y;
    }

    bool operator==(const Point<T>& other) const
    {
        return m_x == other.m_x
            && m_y == other.m_y;
    }

private:
    T m_x {};
    T m_y {};
};

using IntPoint = Point<int>;

} // GUI
