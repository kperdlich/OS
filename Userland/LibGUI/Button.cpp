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
    m_isCurrentlyPressed = true;
}

void Button::onMouseUpEvent(MouseEvent& event)
{
    m_isCurrentlyPressed = false;
    if (onClick)
        onClick();
}

void Button::onMouseMoveEvent(MouseEvent& event)
{
}

void Button::onPaintEvent(Event& event)
{
    static const GUI::Color buttonColor = Colors::Grey;
    Painter painter(this);
    painter.drawFilledRect(rect(), buttonColor);

    if (m_isCurrentlyPressed) {
        painter.drawRect(rect(), Colors::Black);
    } else {
        static const Color shadowGreyColor { 129, 129, 129, 255 };

        const IntPoint topLeft { 0, 0 };
        const IntPoint topRight { m_windowRelativeRect.width(), 0 };
        const IntPoint bottomLeft { 0, m_windowRelativeRect.height() };
        const IntPoint bottomRight { m_windowRelativeRect.width(), m_windowRelativeRect.height() };

        painter.drawLine(topLeft, topRight, Colors::White);
        painter.drawLine(topLeft, bottomLeft, Colors::White);

        painter.drawLine(topRight, bottomRight, Colors::Black);
        painter.drawLine(bottomLeft, bottomRight, Colors::Black);

        painter.drawLine(topRight.x() - 1, topRight.y() + 1, bottomRight.x() - 1, bottomRight.y() - 1, shadowGreyColor);
        painter.drawLine(bottomLeft.x() + 1, bottomLeft.y() - 1, bottomRight.x() - 1, bottomRight.y() - 1, shadowGreyColor);
    }

    Rect textRect = rect();
    if (m_isCurrentlyPressed)
        textRect.moveBy(1, 1);
    painter.drawText(textRect, m_text, Alignment::Center, Colors::Black);
}

void Button::setText(const ADS::String& text)
{
    m_text = text;
}

} // GUI