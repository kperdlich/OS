//
// Created by n3dry on 23.09.22.
//

#include "Button.h"
#include "Painter.h"
#include <utility>

namespace GUI {

Button::Button(Widget* parent)
    : Widget(parent)
{
}

Button::Button(std::function<void()> onClickCallback, Widget* parent)
    : onClick(std::move(onClickCallback))
    , Widget(parent)
{
}

Button::Button(const ADS::String& text, Widget* parent)
    : m_text(text)
    , Widget(parent)
{
}

void Button::onMouseDownEvent(MouseEvent& event)
{
    m_isCurrentlyPressed = true;
    grabMouse();
    update();
}

void Button::onMouseUpEvent(MouseEvent& event)
{
    m_isCurrentlyPressed = false;
    releaseMouse();
    update();

    if (!rect().contains(event.position()))
        return;

    if (onClick)
        onClick();
}

void Button::onMouseMoveEvent(MouseEvent& event)
{
}

void Button::onPaintEvent(PaintEvent& event)
{
    static const GUI::Color buttonColor = Colors::Grey;
    Painter painter(*this);
    painter.setClipRect(event.rect());

    painter.drawFilledRect(rect(), buttonColor);

    if (m_isCurrentlyPressed) {
        painter.drawRect(rect(), Colors::Black);
    } else {
        static const Color shadowGreyColor { 129, 129, 129, 255 };

        const Point topLeft { 0, 0 };
        const Point topRight { width() - 1, 0 };
        const Point bottomLeft { 0, height() - 1 };
        const Point bottomRight { width() - 1, height() - 1 };

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
    update();
}

int Button::fontWidth()
{
    // FIXME: get this from the font
    return 8;
}

Size Button::preferredSizeHint() const
{
    if (m_text.isEmpty())
        return minSizeHint();

    Size size = minSizeHint();
    size.setWidth(ADS::max((static_cast<int>(m_text.length()) * fontWidth()) + (2 * 5), size.width()));
    return size;
}

Size Button::minSizeHint() const
{
    return { 40, 25 };
}

} // GUI