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

void Widget::onPaint()
{
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

void Widget::setRect(const IntRect& rect)
{
    m_rect = rect;
}

void Widget::setWindow(Window* window)
{
    m_window = window;
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