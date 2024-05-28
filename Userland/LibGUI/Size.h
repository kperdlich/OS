//
// Created by n3dry on 19.09.22.
//

#pragma once

namespace GUI {

template<typename T>
class Size {
public:
    Size(T width, T height)
        : m_width(width)
        , m_height(height)
    {
    }

    inline T width() const { return m_width; }
    inline T height() const { return m_height; }

private:
    T m_width {};
    T m_height {};
};

using IntSize = Size<int>;

} // GUI
