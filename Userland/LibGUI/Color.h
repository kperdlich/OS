//
// Created by n3dry on 18.09.22.
//

#pragma once

namespace GUI {

enum class Colors {
    Black,
    White,
    LightWhite,
    Grey,
    DarkGrey,
    Blue,
    DarkBlue,
    Red
};

class Color {
public:
    Color(Colors color);
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

private:
    struct RGBA {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    } m_rgba;
};

} // GUI
