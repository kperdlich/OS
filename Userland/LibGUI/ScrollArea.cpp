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
    m_verticalScrollBar->onValueChanged = [this](int value) {
        m_widget->moveTo(m_widget->relativePosition().x(), -value);
    };
    m_horizontalScrollBar = new ScrollBar(Orientation::Horizontal, this);
    m_horizontalScrollBar->onValueChanged = [this](int value) {
        m_widget->moveTo(-value, m_widget->relativePosition().y());
    };
}

void ScrollArea::onResizeEvent(ResizeEvent& event)
{
    m_verticalScrollBar->setRelativeRect(
        { event.size().width() - m_verticalScrollBar->preferredSizeHint().width(),
            0,
            m_verticalScrollBar->preferredSizeHint().width(),
            event.size().height() - m_horizontalScrollBar->height() });

    m_horizontalScrollBar->setRelativeRect(
        { 0,
            event.size().height() - m_horizontalScrollBar->preferredSizeHint().height(),
            event.size().width() - m_verticalScrollBar->width(),
            m_horizontalScrollBar->preferredSizeHint().height() });

    updateWidgetSize();
    updateScrollBars();
}

void ScrollArea::updateScrollBars()
{
    const Size availableSize = availableContentSize();
    const Size contentSize = m_widget->size();
    const int exceededHeight = ADS::max(contentSize.height() - availableSize.height(), 0);
    const int exceededWidth = ADS::max(contentSize.width() - availableSize.width(), 0);

    if (exceededHeight > 0) {
        const int step = availableSize.height() - exceededHeight;
        m_verticalScrollBar->setPageStep(step);
        m_verticalScrollBar->setIsVisible(true);
        m_verticalScrollBar->setRange(0, exceededHeight);
    } else {
        m_verticalScrollBar->setIsVisible(false);
    }

    if (exceededWidth > 0) {
        const int step = availableSize.width() - exceededWidth;
        m_horizontalScrollBar->setPageStep(step);
        m_horizontalScrollBar->setIsVisible(true);
        m_horizontalScrollBar->setRange(0, exceededWidth);
    } else {
        m_horizontalScrollBar->setIsVisible(false);
    }
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

Size ScrollArea::availableContentSize() const
{
    Size availableSize = size();
    if (m_widget->size().width() > size().width())
        availableSize.setHeight(availableSize.height() - m_horizontalScrollBar->size().height());
    if (m_widget->size().height() > size().height())
        availableSize.setWidth(availableSize.width() - m_verticalScrollBar->size().width());
    return availableSize;
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