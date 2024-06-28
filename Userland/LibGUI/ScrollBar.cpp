//
// Created by n3dry on 20.06.24.
//

#include "ScrollBar.h"
#include "Painter.h"

namespace GUI {

ScrollBar::ScrollBar(Orientation orientation, Widget* parent)
    : m_orientation(orientation)
    , Widget(parent)
{
}

void ScrollBar::onPaintEvent(PaintEvent& event)
{
    Painter painter(this);
    painter.setClipRect(event.rect());

    painter.drawFilledRect(rect(), Colors::White);
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
    update();
}

void ScrollBar::onMouseDownEvent(MouseEvent& event)
{
    if (scrollUpButtonRect().contains(event.position())) {
        const int oldValue = m_sliderValue;
        m_sliderValue = ADS::max(m_sliderValue - m_singleStep, m_min);
        if (m_sliderValue != oldValue && onValueChanged)
            onValueChanged(m_sliderValue);
        update();
        return;
    }

    if (scrollDownButtonRect().contains(event.position())) {
        const int oldValue = m_sliderValue;
        m_sliderValue = ADS::min(m_sliderValue + m_singleStep, m_max);
        if (m_sliderValue != oldValue && onValueChanged)
            onValueChanged(m_sliderValue);
        update();
        return;
    }

    if (sliderRect().contains(event.position())) {
        m_isDragging = true;
        m_draggingStartPosition = event.position();
        m_draggingStartValue = value();
        grabMouse();
        update();
        return;
    }

    if (sliderRect().y() < event.y()) {
        const int oldValue = m_sliderValue;
        m_sliderValue = ADS::min(m_sliderValue + m_pageStep, m_max);
        if (m_sliderValue != oldValue && onValueChanged)
            onValueChanged(m_sliderValue);
        update();
    }
    else {
        const int oldValue = m_sliderValue;
        m_sliderValue = ADS::max(m_sliderValue - m_pageStep, m_min);
        if (m_sliderValue != oldValue && onValueChanged)
            onValueChanged(m_sliderValue);
        update();
    }
}

void ScrollBar::onMouseUpEvent(MouseEvent& event)
{
    m_isDragging = false;
    releaseMouse();
    update();
}

void ScrollBar::setRange(int min, int max)
{
    ASSERT(max > min);
    m_min = min;
    m_max = max;

    const int oldValue = m_sliderValue;
    m_sliderValue = ADS::clamp(m_sliderValue, m_min, m_max);
    if (m_sliderValue != oldValue && onValueChanged)
        onValueChanged(m_sliderValue);
    update();
}

void ScrollBar::setValue(int value)
{
    if (value == m_sliderValue)
        return;

    const int oldValue = m_sliderValue;
    m_sliderValue = ADS::clamp(value, m_min, m_max);
    if (m_sliderValue != oldValue && onValueChanged)
        onValueChanged(m_sliderValue);
    update();
}

int ScrollBar::calculateDraggingDelta(const IntPoint& newPosition) const
{
    const int positionDelta = m_orientation == Orientation::Vertical
        ? newPosition.y() - m_draggingStartPosition.y()
        : newPosition.x() - m_draggingStartPosition.x();
    const int range = m_max - m_min;
    const int sliderRange = availableRange() - sliderLength();
    ASSERT(sliderRange >= 0);
    return (positionDelta * range) / sliderRange;
}

int ScrollBar::sliderLength() const
{
    const int range = m_max - m_min;
    ASSERT(range > 0);
    const int length = (m_pageStep * availableRange()) / range;
    return length;
}

int ScrollBar::availableRange() const
{
    if (m_orientation == Orientation::Vertical)
        return height() - scrollUpButtonRect().height() - scrollDownButtonRect().height();

    return width() - scrollUpButtonRect().width() - scrollDownButtonRect().width();
}

Rect ScrollBar::scrollUpButtonRect() const
{
    return { 0, 0, scrollButtonSize(), scrollButtonSize() };
}

Rect ScrollBar::scrollDownButtonRect() const
{
    if (m_orientation == Orientation::Vertical)
        return { 0, height() - scrollButtonSize(), scrollButtonSize(), scrollButtonSize() };

    return { width() - scrollButtonSize(), 0, scrollButtonSize(), scrollButtonSize() };
}

Rect ScrollBar::sliderRect() const
{
    const int range = m_max - m_min;
    ASSERT(range >= 0);
    const int sliderRange = availableRange() - sliderLength();
    const int sliderPos = ((m_sliderValue - m_min) * sliderRange) / range;
    if (m_orientation == Orientation::Vertical)
        return { 0, scrollUpButtonRect().height() + sliderPos, preferredSizeHint().width(), sliderLength() };

    return { scrollUpButtonRect().width() + sliderPos, 0, sliderLength(), preferredSizeHint().height() };
}

Size ScrollBar::preferredSizeHint() const
{
    if (m_orientation == Orientation::Vertical)
        return { 15, 0 };

    return { 0, 15 };
}

Size ScrollBar::minSizeHint() const
{
    return Size::Invalid();
}

} // GUI