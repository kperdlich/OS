//
// Created by n3dry on 17.09.22.
//

#include "Window.h"
#include "Painter.h"
#include "WindowManager.h"

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
    if (!m_centralWidget)
        return;

    Widget::HitResult result {};
    if (m_centralWidget->hits(x, y, result)) {
        result.widget->onMouseMove(x, y);
    }
}

void Window::onMouseUp(int key, int x, int y)
{
    if (!m_centralWidget)
        return;

    Widget::HitResult result {};
    if (m_centralWidget->hits(x, y, result)) {
        result.widget->onMouseUp(key, x, y);
    }
}

void Window::onMouseDown(int key, int x, int y)
{
    if (!m_centralWidget)
        return;

    Widget::HitResult result {};
    if (m_centralWidget->hits(x, y, result)) {
        std::cout << "[Widget::HitResult] " << result.widget->name() << " localX: " << result.localX << " localY: " << result.localY << std::endl;
        result.widget->onMouseDown(key, x, y);
    }
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
    m_centralWidget->setParent(this);
    m_centralWidget->setWindow(this);
}

void Window::show()
{
    GUI::WindowManager::the().add(*this);
}

void Window::onKeyDown(const KeyEvent& event)
{
    if (m_focusedWidget) {
        m_focusedWidget->onKeyDown(event);
    }
}

void Window::onKeyUp(const KeyEvent& event)
{
    if (m_focusedWidget) {
        m_focusedWidget->onKeyUp(event);
    }
}

void Window::setFocusedWidget(Widget* widget)
{
    if (m_focusedWidget == widget)
        return;

    // FIXME: handle focus event on old and new widget
    m_focusedWidget = widget;
}

} // GUI