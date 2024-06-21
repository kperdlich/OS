//
// Created by n3dry on 20.06.24.
//

#include "ScrollBar.h"
#include "Painter.h"

namespace GUI {

ScrollBar::ScrollBar(Widget* parent)
    : Widget(parent)
{
}

void ScrollBar::onPaintEvent(Event& event)
{
    Painter painter(this);
    painter.drawRect(rect(), Colors::Black);

    painter.drawFilledRect(scrollUpButtonRect(), Colors::DarkGrey);
    painter.drawRect(scrollUpButtonRect(), Colors::Black);

    painter.drawFilledRect(scrollDownButtonRect(), Colors::DarkGrey);
    painter.drawRect(scrollDownButtonRect(), Colors::Black);

    painter.drawFilledRect(sliderRect(), Colors::DarkGrey);
    painter.drawRect(sliderRect(), Colors::Black);
}

void ScrollBar::onMouseMoveEvent(MouseEvent& event)
{
    if (!m_isDragging)
        return;

    const int delta = calculateDraggingDelta(event.position());
    const int newSliderValue = ADS::clamp(m_draggingStartValue + delta, m_min, m_max);
    if (newSliderValue == m_sliderValue)
        return;

    m_sliderValue = newSliderValue;
    if (onValueChanged)
        onValueChanged(m_sliderValue);
}

void ScrollBar::onMouseDownEvent(MouseEvent& event)
{
    if (scrollUpButtonRect().contains(event.position())) {
        m_sliderValue = ADS::max(m_sliderValue - m_singleStep, m_min);
        return;
    }

    if (scrollDownButtonRect().contains(event.position())) {
        m_sliderValue = ADS::min(m_sliderValue + m_singleStep, m_max);
        return;
    }

    if (sliderRect().contains(event.position())) {
        m_isDragging = true;
        m_draggingStartPosition = event.position();
        m_draggingStartValue = value();
        grabMouse();
        return;
    }

    if (sliderRect().y() < event.y())
        m_sliderValue = ADS::min(m_sliderValue + m_pageStep, m_max);
    else
        m_sliderValue = ADS::max(m_sliderValue - m_pageStep, m_min);
}

void ScrollBar::onMouseUpEvent(MouseEvent& event)
{
    m_isDragging = false;
    releaseMouse();
}

int ScrollBar::calculateDraggingDelta(const IntPoint& newPosition) const
{
    const int positionDelta = newPosition.y() - m_draggingStartPosition.y();
    const int range = m_max - m_min;
    const int sliderRange = height() - scrollUpButtonRect().height() - scrollDownButtonRect().height() - sliderLength();
    ASSERT(sliderRange >= 0);
    return (positionDelta * range) / sliderRange;
}

int ScrollBar::sliderLength() const
{
    const int range = m_max - m_min;
    const int availableSize = height() - scrollUpButtonRect().height() - scrollDownButtonRect().height();
    ASSERT(range >= 0);
    const int length = (m_pageStep * availableSize) / range;
    return length;
}

Size ScrollBar::preferredSizeHint() const
{
    // FIXME: depends on direction
    return { 15, 15 };
}

Size ScrollBar::minSizeHint() const
{
    return Size::Invalid();
}

Rect ScrollBar::scrollUpButtonRect() const
{
    return { 0, 0, width(), width() };
}

Rect ScrollBar::scrollDownButtonRect() const
{
    return { 0, height() - width(), width(), width() };
}

Rect ScrollBar::sliderRect() const
{
    const int range = m_max - m_min;
    ASSERT(range >= 0);
    const int sliderRange = height() - scrollUpButtonRect().height() - scrollDownButtonRect().height() - sliderLength();
    const int sliderPos = ((m_sliderValue - m_min) * sliderRange) / range;
    return { 0, scrollUpButtonRect().height() + sliderPos, preferredSizeHint().width(), sliderLength() };
}

} // GUI