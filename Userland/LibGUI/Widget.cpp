//
// Created by n3dry on 17.09.22.
//

#include "Widget.h"
#include "Painter.h"

namespace GUI {
Widget::Widget(Widget* parent)
    : CObject(parent)
{
}

void Widget::onPaint()
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