//
// Created by n3dry on 17.09.22.
//

#include "Widget.h"

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
    // FIXME: This should be done in the event function and not the virtual callbacks
    m_hasFocus = true;
    focusInEvent();
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

} // GUI