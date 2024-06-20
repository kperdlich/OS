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

    /*const int sliderLengthInPixel = height() - scrollDownButtonRect().height() - scrollUpButtonRect().height();
    const int amountOfSteps = (m_max - m_min) / sliderLengthInPixel;
    const int newSliderValue = ADS::clamp(sliderLengthInPixel / event.y(), 0, amountOfSteps + 1);
    const int newValue = m_max - (newSliderValue * m_stepSize);
    m_value = newValue;
    const int delta = event.y() - m_draggingStart.y();
    std::cout << "delta: " << delta << " steps: " << amountOfSteps <<  std::endl;*/
}

void ScrollBar::onMouseDownEvent(MouseEvent& event)
{
    if (scrollUpButtonRect().contains(event.position())) {
        doSingleStepUp();
        return;
    }

    if (scrollDownButtonRect().contains(event.position())) {
        doSingleStepDown();
        return;
    }

    if (sliderRect().contains(event.position())) {
        m_isDragging = true;
        m_draggingStart = event.position();
        return;
    }
}

void ScrollBar::onMouseUpEvent(MouseEvent& event)
{
    m_isDragging = false;
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
    const int sliderLengthInPixel = height() - scrollDownButtonRect().height() - scrollUpButtonRect().height();
    const int amountOfSteps = (m_max - m_min) / m_stepSize;
    const int sliderHeight = sliderLengthInPixel / amountOfSteps;

    return Rect(0, scrollUpButtonRect().height() + value(), preferredSizeHint().width(), sliderHeight);
}

void ScrollBar::doSingleStepUp()
{
    m_value = ADS::max(m_value - 1, m_min);
}

void ScrollBar::doSingleStepDown()
{
    m_value = ADS::min(m_value + 1, m_max);
}

} // GUI