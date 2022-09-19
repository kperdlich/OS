//
// Created by n3dry on 19.09.22.
//

#pragma once

namespace GUI {

template<typename T>
class Size {
public:
    Size(T with, T height)
        : m_width(with)
        , m_height(height)
    {
    }

    inline T width() { return m_width; }
    inline T height() { return m_height; }

private:
    T m_width;
    T m_height;
};

using IntSize = Size<int>;

} // GUI
