//
// Created by n3dry on 17.09.22.
//

#include "Window.h"

namespace GUI {

Window::Window(IntRect rect)
    : m_rect(rect)
{
}

void Window::render(Painter& painter)
{
    if (!m_isVisible)
        return;

    painter.drawRectangle({ m_rect.x(), m_rect.y(), 800, 600 }, GUI::Color(0x00, 0, 0, 0xff));
    painter.drawRectangle({ m_rect.x(), m_rect.y(), 800, 20 }, GUI::Color(0, 0x00, 0xff, 0xff));
    painter.drawRectangle({ m_rect.x() + m_rect.width() - 20, m_rect.y(), 20, 20 }, GUI::Color(0xff, 0, 0, 0xff));
}

void Window::onMouseMove(int x, int y)
{
    if (!m_rect.isPointInside(x, y))
        return;

    if (m_isDragging) {
        const int deltaX = x - m_lastMouseMovePos.width();
        const int deltaY = y - m_lastMouseMovePos.height();
        m_rect.moveBy(deltaX, deltaY);
        m_lastMouseMovePos = { x, y };
    }
}

void Window::onMouseUp(int key, int x, int y)
{
    if (m_rect.isPointInside(x, y)) {
        m_isDragging = false;
    }
}

void Window::onMouseDown(int key, int x, int y)
{
    if (m_rect.isPointInside(x, y)) {
        m_lastMouseMovePos = { x, y };
        m_isDragging = true;
    }
}

} // GUI