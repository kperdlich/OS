//
// Created by n3dry on 23.09.22.
//

#include "Button.h"
#include "Painter.h"
#include <utility>

namespace GUI {

Button::Button(std::function<void()> onClickCallback, Widget* parent)
    : onClick(std::move(onClickCallback))
    , Widget(parent)
{
}

void Button::onMouseDownEvent(MouseEvent& event)
{
}

void Button::onMouseUpEvent(MouseEvent& event)
{
    if (onClick)
        onClick();
}

void Button::onMouseMoveEvent(MouseEvent& event)
{
}

void Button::onPaintEvent(Event& event)
{
    static const GUI::Color buttonColor = Colors::Grey;
    static const Color shadowGreyColor { 129, 129, 129, 255 };

    Painter painter(this);

    const IntPoint topLeft { 0, 0 };
    const IntPoint topRight { m_windowRelativeRect.width(), 0 };
    const IntPoint bottomLeft { 0, m_windowRelativeRect.height() };
    const IntPoint bottomRight { m_windowRelativeRect.width(), m_windowRelativeRect.height() };

    painter.drawFilledRect(rect(), buttonColor);

    painter.drawLine(topLeft.x(), topLeft.y(), topRight.x(), topRight.y(), Colors::White);
    painter.drawLine(topLeft.x(), topLeft.y(), bottomLeft.x(), bottomLeft.y(), Colors::White);

    painter.drawLine(topRight.x(), topRight.y(), bottomRight.x(), bottomRight.y(), Colors::Black);
    painter.drawLine(bottomLeft.x(), bottomLeft.y(), bottomRight.x(), bottomRight.y(), Colors::Black);

    painter.drawLine(topRight.x() - 1, topRight.y() + 1, bottomRight.x() - 1, bottomRight.y() - 1, shadowGreyColor);
    painter.drawLine(bottomLeft.x() + 1, bottomLeft.y() - 1, bottomRight.x() - 1, bottomRight.y() - 1, shadowGreyColor);

    painter.drawText(rect(), m_text, Alignment::Center, Colors::Black);
}

void Button::setText(const ADS::String& text)
{
    m_text = text;
}

} // GUI