//
// Created by n3dry on 20.06.24.
//

#include "ScrollArea.h"
#include "Layout.h"
#include "Painter.h"

namespace GUI {

ScrollArea::ScrollArea(Widget* parent)
    : Widget(parent)
{
    m_verticalScrollBar = new ScrollBar(Orientation::Vertical, this);
    m_horizontalScrollBar = new ScrollBar(Orientation::Horizontal, this);
}

void ScrollArea::onResizeEvent(ResizeEvent& event)
{
    const int heightRequiredByHorizontalScrollBar = m_horizontalScrollBar->isVisible()
        ? m_horizontalScrollBar->height()
        : 0;
    const int widthRequiredByVerticalScrollBar = m_verticalScrollBar->isVisible()
        ? m_verticalScrollBar->width()
        : 0;

    m_verticalScrollBar->setRelativeRect(
        {  event.size().width() - m_verticalScrollBar->preferredSizeHint().width(),
            0,
            m_verticalScrollBar->preferredSizeHint().width(),
            event.size().height() - heightRequiredByHorizontalScrollBar });

    m_horizontalScrollBar->setRelativeRect(
        { 0,
            event.size().height() - m_horizontalScrollBar->preferredSizeHint().height(),
            event.size().width() - widthRequiredByVerticalScrollBar,
            m_horizontalScrollBar->preferredSizeHint().height() });

    updateWidgetSize();
}

void ScrollArea::updateWidgetSize()
{
    if (!m_widget)
        return;

    const Size newSize = m_widget->preferredSizeHint();
    m_widget->resize(newSize);
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

void ScrollArea::setWidget(Widget* widget)
{
    if (m_widget == widget)
        return;

    if (m_widget)
        removeChild(*m_widget);

    m_widget = widget;

    if (m_widget)
        m_widget->setParent(this);

    updateWidgetSize();
}

Size ScrollArea::preferredSizeHint() const
{
    if (!m_widget)
        return Widget::preferredSizeHint();

    Size size = m_widget->preferredSizeHint();
    if (m_widget->verticalSizePolicy() == Widget::SizePolicy::Fixed)
        size.setHeight(m_widget->minimumSize().height());
    if (m_widget->horizontalSizePolicy() == Widget::SizePolicy::Fixed)
        size.setWidth(m_widget->minimumSize().width());

    size += m_horizontalScrollBar->preferredSizeHint();
    size += m_verticalScrollBar->preferredSizeHint();
    return size;
}

Size ScrollArea::minSizeHint() const
{
    return Widget::minSizeHint();
}

} // GUI