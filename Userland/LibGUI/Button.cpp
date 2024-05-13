//
// Created by n3dry on 23.09.22.
//

#include "Button.h"
#include <utility>

namespace GUI {

Button::Button(std::function<void()> onClickCallback, Widget* parent)
    : onClick(std::move(onClickCallback))
    , Widget(parent)
{
}

void Button::onMouseDown(int key, int x, int y)
{
}

void Button::onMouseUp(int key, int x, int y)
{
    if (onClick)
        onClick();
}

void Button::onMouseMove(int x, int y)
{
}

void Button::onPaint()
{
    //painter.drawRectangle(m_rect, Color {0, 0xff, 0, 0xff});
}

} // GUI