//
// Created by n3dry on 17.09.22.
//

#pragma once

namespace GUI {

class Window final {
public:

    Window();
    ~Window() = default;

private:
    int m_width = 0;
    int m_height = 0;
    int m_position_x = 0;
    int m_position_y = 0;
};

} // GUI
