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
    m_verticalScrollBar = ADS::makeOwn<ScrollBar>(Orientation::Vertical, this);
    m_verticalScrollBar->onValueChanged = [this](int value) {
        m_widget->moveTo(m_widget->relativePosition().x(), -value);
        update();
    };
    m_horizontalScrollBar = ADS::makeOwn<ScrollBar>(Orientation::Horizontal, this);
    m_horizontalScrollBar->onValueChanged = [this](int value) {
        m_widget->moveTo(-value, m_widget->relativePosition().y());
        update();
    };
}

void ScrollArea::onResizeEvent(ResizeEvent& event)
{
    const int heightForHorizontalScrollBar = m_horizontalScrollBar->isVisible() ? m_horizontalScrollBar->height() : 0;
    const int widthForVerticalScrollBar = m_verticalScrollBar->isVisible() ? m_verticalScrollBar->width() : 0;

    m_verticalScrollBar->setRelativeRect(
        { event.size().width() - m_verticalScrollBar->preferredSizeHint().width(),
            0,
            m_verticalScrollBar->preferredSizeHint().width(),
            event.size().height() - heightForHorizontalScrollBar });

    m_horizontalScrollBar->setRelativeRect(
        { 0,
            event.size().height() - m_horizontalScrollBar->preferredSizeHint().height(),
            event.size().width() - widthForVerticalScrollBar,
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

    m_verticalScrollBar->setPageStep(visibleContentRect().height());
    m_verticalScrollBar->setIsVisible(true);
    m_verticalScrollBar->setRange(0, exceededHeight);
    m_verticalScrollBar->setIsVisible(exceededHeight > 0);

    m_horizontalScrollBar->setPageStep(visibleContentRect().width());
    m_horizontalScrollBar->setIsVisible(true);
    m_horizontalScrollBar->setRange(0, exceededWidth);
    m_horizontalScrollBar->setIsVisible(exceededWidth > 0);
}

void ScrollArea::updateWidgetSize()
{
    if (!m_widget)
        return;

    const Size newSize = m_widget->preferredSizeHint();
    m_widget->resize(newSize);
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
    if (m_widget)
        return m_widget->minSizeHint();

    return Widget::minSizeHint();
}

void ScrollArea::onPaintEvent(PaintEvent& event)
{
#if 0
    Painter painter(*this);
    painter.setClipRect(event.rect());
    painter.drawFilledRect(rect(), Colors::Blue);
#endif
}

Rect ScrollArea::visibleContentRect() const
{
    Size contentSize = size();
    contentSize.setWidth(contentSize.width() - (m_verticalScrollBar->isVisible() ? m_verticalScrollBar->size().width() : 0));
    contentSize.setHeight(contentSize.height() - (m_horizontalScrollBar->isVisible() ? m_horizontalScrollBar->size().height() : 0));

    return {
        m_horizontalScrollBar->value(),
        m_verticalScrollBar->value(),
        contentSize
    };
}

} // GUI