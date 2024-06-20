//
// Created by n3dry on 20.06.24.
//

#include "ScrollArea.h"
#include "Painter.h"

namespace GUI {

ScrollArea::ScrollArea(Widget* parent)
    : Widget(parent)
{
    m_verticalScrollBar = new ScrollBar(this);
}

void ScrollArea::onResizeEvent(ResizeEvent& event)
{
    m_verticalScrollBar->setWindowRelativeRect({ event.size().width() - m_verticalScrollBar->preferredSizeHint().width() - 1, 0, m_verticalScrollBar->preferredSizeHint().width(), event.size().height() - 2 });
}

void ScrollArea::onPaintEvent(Event& event)
{
    Painter painter(this);
}

void ScrollArea::onMouseMoveEvent(MouseEvent& event)
{
    Widget::onMouseMoveEvent(event);
}

void ScrollArea::onMouseDownEvent(MouseEvent& event)
{
    Widget::onMouseDownEvent(event);
}

void ScrollArea::onMouseUpEvent(MouseEvent& event)
{
    Widget::onMouseUpEvent(event);
}

void ScrollArea::setWidget(Widget& widget)
{
    widget.setParent(this);
    m_widget = &widget;
}

Size ScrollArea::preferredSizeHint() const
{
    if (!m_contentSize.isInvalid())
        return m_contentSize;

    if (m_widget)
        return m_widget->preferredSizeHint();
    return Widget::preferredSizeHint();
}

Size ScrollArea::minSizeHint() const
{
    return Widget::minSizeHint();
}

void ScrollArea::setContentSize(Size contentSize)
{
    m_contentSize = contentSize;
}

} // GUI