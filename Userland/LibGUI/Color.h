//
// Created by n3dry on 18.09.22.
//

#pragma once

namespace GUI {

class Color {
public:
    explicit Color(unsigned char value);
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

private:
    unsigned char m_r, m_g, m_b, m_a;
};

} // GUI
