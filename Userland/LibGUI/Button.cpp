//
// Created by n3dry on 23.09.22.
//

#include "Button.h"

#include <utility>

namespace GUI {

Button::Button(Widget* parent, std::function<void()> onClickCallback)
    : m_onClick(std::move(onClickCallback))
    , Widget(parent)
{
}

void Button::onMouseDown(int key, int x, int y)
{
}

void Button::onMouseUp(int key, int x, int y)
{
    if (m_onClick)
        m_onClick();
}

void Button::render(Painter& painter, IntRect rect, Color color)
{
    painter.drawRectangle(rect, color);
}

void Button::onMouseMove(int x, int y)
{
}

} // GUI