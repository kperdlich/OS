//
// Created by n3dry on 17.09.22.
//

#include "Widget.h"
#include "Window.h"

namespace GUI {
Widget::Widget(Widget* parent)
    : CObject(parent)
{
}

bool Widget::hits(int x, int y, HitResult& result)
{
    for (auto& child : m_children) {
        Widget* childWidget = static_cast<Widget*>(child);
        if (childWidget->hits(x - childWidget->windowRelativeRect().x(), y - childWidget->windowRelativeRect().y(), result)) {
            return true;
        }
    }

    if (m_windowRelativeRect.contains(x, y)) {
        result.widget = this;
        result.localX = x;
        result.localY = y;
        return true;
    }

    return false;
}

void Widget::onPaint()
{
    for (auto& child : m_children) {
        Widget* childWidget = static_cast<Widget*>(child);
        childWidget->onPaint();
    }
}

void Widget::onMouseMove(int x, int y)
{
}

void Widget::onMouseDown(int key, int x, int y)
{
    setFocus();
}

void Widget::onMouseUp(int key, int x, int y)
{
}

void Widget::focusInEvent()
{
}

void Widget::focusOutEvent()
{
}

void Widget::setWindowRelativeRect(const IntRect& rect)
{
    m_windowRelativeRect = rect;
}

void Widget::setWindow(Window* window)
{
    m_window = window;
    for (auto& child : m_children) {
        Widget* childWidget = static_cast<Widget*>(child);
        childWidget->setWindow(window);
    }
}

bool Widget::hasFocus() const
{
    ASSERT(m_window != nullptr);
    return m_window->focusedWidget() == this;
}

void Widget::setFocus()
{
    ASSERT(m_window != nullptr);
    m_window->setFocusedWidget(this);
}

void Widget::onKeyDown(const KeyEvent& event)
{
}

void Widget::onKeyUp(const KeyEvent& event)
{
}

} // GUI