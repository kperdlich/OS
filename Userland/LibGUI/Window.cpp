//
// Created by n3dry on 17.09.22.
//

#include "Window.h"
#include "WindowStack.h"

namespace GUI {

Window::Window(IntRect rect)
    : m_rect(rect)
{
    m_closeButton = new Button(nullptr, [&]() {
        GUI::WindowStack::the().remove(this);
    });

    m_hideButton = new Button(nullptr, [&]() {
        hide();
    });
}

void Window::render(Painter& painter)
{
    if (!m_isVisible)
        return;

    painter.drawRectangle({ m_rect.x(), m_rect.y(), 800, 600 }, GUI::Color(0x00, 0, 0, 0xff));
    painter.drawRectangle({ m_rect.x(), m_rect.y(), 800, 20 }, GUI::Color(0, 0x00, 0xff, 0xff));
    m_closeButton->render(painter, { m_rect.x() + m_rect.width() - 20, m_rect.y(), 20, 20 }, Color { 0xff, 0, 0, 0xff });
    m_closeButton->render(painter, { m_rect.x() + m_rect.width() - 40, m_rect.y(), 20, 20 }, Color { 0, 0xff, 0, 0xff });
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

    if (IntRect { m_rect.x() + m_rect.width() - 20, m_rect.y(), 20, 20 }.isPointInside(x, y)) {
        m_closeButton->onMouseMove(x, y);
    }

    if (IntRect { m_rect.x() + m_rect.width() - 40, m_rect.y(), 20, 20 }.isPointInside(x, y)) {
        m_hideButton->onMouseMove(x, y);
    }
}

void Window::onMouseUp(int key, int x, int y)
{
    if (m_rect.isPointInside(x, y)) {
        m_isDragging = false;
    }

    if (IntRect { m_rect.x() + m_rect.width() - 20, m_rect.y(), 20, 20 }.isPointInside(x, y)) {
        m_closeButton->onMouseUp(key, x, y);
    }

    if (IntRect { m_rect.x() + m_rect.width() - 40, m_rect.y(), 20, 20 }.isPointInside(x, y)) {
        m_hideButton->onMouseUp(key, x, y);
    }
}

void Window::onMouseDown(int key, int x, int y)
{
    if (m_rect.isPointInside(x, y)) {
        m_lastMouseMovePos = { x, y };
        m_isDragging = true;
    }

    if (IntRect { m_rect.x() + m_rect.width() - 20, m_rect.y(), 20, 20 }.isPointInside(x, y)) {
        m_closeButton->onMouseDown(key, x, y);
    }

    if (IntRect { m_rect.x() + m_rect.width() - 40, m_rect.y(), 20, 20 }.isPointInside(x, y)) {
        m_hideButton->onMouseDown(key, x, y);
    }
}
bool Window::hits(int x, int y)
{
    return m_rect.isPointInside(x, y);
}

} // GUI