//
// Created by n3dry on 23.09.22.
//

#include "Button.h"
#include "Painter.h"
#include <utility>

namespace GUI {

static const GUI::Color ButtonColor { 172, 172, 172, 0xff };

Button::Button(std::function<void()> onClickCallback, Widget* parent)
    : onClick(std::move(onClickCallback))
    , Widget(parent)
{
}

void Button::onMouseDown(int key, int x, int y)
{
    std::cout << "[Button] onMouseDown" << std::endl;
}

void Button::onMouseUp(int key, int x, int y)
{
    std::cout << "[Button] onMouseUp" << std::endl;
    if (onClick)
        onClick();
}

void Button::onMouseMove(int x, int y)
{
    std::cout << "[Button] onMouseMove" << std::endl;
}

void Button::onPaint()
{
    Painter painter(this);
    painter.drawFilledRect(m_rect, ButtonColor);
    painter.drawRect(m_rect, Color { 0, 0, 0, 0xff });

    painter.drawText(m_rect, m_text, GUI::TextAlignment::Center, Color { 0, 0, 0, 0xff });
}

void Button::setText(const ADS::String& text)
{
    m_text = text;
}

} // GUI