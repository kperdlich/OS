//
// Created by n3dry on 17.09.22.
//

#include "Window.h"
#include "WindowManager.h"

namespace GUI {

Window::Window(IntRect rect)
    : m_rect(rect)
{
}

void Window::onMouseMove(int x, int y)
{
}

void Window::onMouseUp(int key, int x, int y)
{
}

void Window::onMouseDown(int key, int x, int y)
{
}

bool Window::contains(int x, int y)
{
    return m_rect.contains(x, y);
}

void Window::moveBy(int x, int y)
{
    m_rect.moveBy(x, y);
}

} // GUI