//
// Created by n3dry on 17.09.22.
//

#include "Window.h"

namespace GUI {
Window::Window(Rect size)
    : m_size(size)
{
}

void Window::render(Painter& painter)
{
    if (!m_isVisible)
        return;

    painter.drawRectangle(m_position_x, m_position_y, { 800, 600 }, GUI::Color(0x00, 0, 0, 0xff));
    painter.drawRectangle(m_position_x, m_position_y, { 800, 20 }, GUI::Color(0, 0x00, 0xff, 0xff));
    painter.drawRectangle(m_position_x + m_size.width - 20, 0, { 20, 20 }, GUI::Color(0xff, 0, 0, 0xff));
}
} // GUI