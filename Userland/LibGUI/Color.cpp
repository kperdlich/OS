//
// Created by n3dry on 18.09.22.
//

#include "Color.h"

namespace Gui {
Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    : m_r(r)
    , m_g(g)
    , m_b(b)
    , m_a(a)
{
}
Color::Color(unsigned char value)
    : m_r(value)
    , m_g(value)
    , m_b(value)
    , m_a(value)
{
}
} // Gui