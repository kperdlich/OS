//
// Created by n3dry on 23.09.22.
//

#include "Button.h"
#include "Painter.h"
#include <utility>

namespace GUI {

static const GUI::Color ButtonColor = Colors::Grey;

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
    Painter painter(this);
    painter.drawFilledRect(m_windowRelativeRect, ButtonColor);
    painter.drawRect(m_windowRelativeRect, Colors::Black);

    painter.drawText(m_windowRelativeRect, m_text, Alignment::Center, Colors::Black);

    Widget::onPaintEvent(event);
}

void Button::setText(const ADS::String& text)
{
    m_text = text;
}

} // GUI