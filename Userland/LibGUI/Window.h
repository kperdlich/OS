//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "Painter.h"
#include "Rect.h"

namespace GUI {

class Window final {
public:
    Window(Rect size);
    ~Window() = default;

    inline void show() { m_isVisible = true; }
    inline void hide() { m_isVisible = false; }

    void render(Painter& painter);

private:
    bool m_isVisible = true;
    Rect m_size;
    int m_position_x = 0;
    int m_position_y = 0;
};

} // GUI
