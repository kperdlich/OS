//
// Created by n3dry on 17.09.22.
//

#include "Window.h"
#include "WindowManager.h"
#include "Painter.h"

namespace GUI {

Window::Window(CObject* parent)
    : CObject(parent)
{
}

Window::~Window()
{
    if (m_centralWidget) {
        delete m_centralWidget;
        m_centralWidget = nullptr;
    }

    GUI::WindowManager::the().remove(*this);
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

void Window::onPaint()
{
    if (m_centralWidget) {
        m_centralWidget->onPaint();
    }
}

bool Window::contains(int x, int y)
{
    return m_rect.contains(x, y);
}

void Window::moveBy(int x, int y)
{
    m_rect.moveBy(x, y);
}

void Window::setRect(const IntRect& rect)
{
    // FIXME: Handle resize event?
    m_rect = rect;
}

void Window::setCentralWidget(Widget& widget)
{
    if (&widget == m_centralWidget) {
        return;
    }

    if (m_centralWidget) {
        m_centralWidget->setParent(nullptr);
    }

    m_centralWidget = &widget;

    if (m_centralWidget) {
        m_centralWidget->setParent(this);
    }
}

void Window::show()
{
    GUI::WindowManager::the().add(*this);
}

} // GUI