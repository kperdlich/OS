//
// Created by n3dry on 18.09.22.
//

#include "Color.h"

namespace GUI {
Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    : m_rgba { r, g, b, a }
{
}

Color::Color(Colors color)
{
    switch (color) {
    case Colors::Black:
        m_rgba = { 0, 0, 0, 255 };
        break;
    case Colors::White:
        m_rgba = { 255, 255, 255, 255 };
        break;
    case Colors::Red:
        m_rgba = { 255, 0, 0, 255 };
        break;
    case Colors::LightWhite:
        m_rgba = { 234, 233, 233, 255 };
        break;
    case Colors::Grey:
        m_rgba = { 172, 172, 172, 255 };
        break;
    case Colors::DarkGrey:
        m_rgba = { 140, 140, 140, 255 };
        break;
    case Colors::Blue:
        m_rgba = { 0, 0, 255, 255 };
        break;
    case Colors::DarkBlue:
        m_rgba = { 0, 0, 104, 255 };
        break;
    case Colors::Green:
        m_rgba = { 0, 255, 0, 255 };
        break;
    }
}

} // GUI