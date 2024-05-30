//
// Created by Kevin on 29.05.24.
//

#include "TextBox.h"
#include "Painter.h"

namespace GUI {

TextBox::TextBox(Widget* parent)
    : Widget(parent)
{
}

TextBox::TextBox(const ADS::String& text, Widget* parent)
    : m_text(text)
    , Widget(parent)
{
}

void TextBox::onPaint()
{
    static const int cursorMargin = 5;

    Painter painter(this);
    painter.drawFilledRect(m_rect, Colors::White);
    painter.drawRect(m_rect, Colors::Black);

    IntRect textRect = m_rect;
    textRect.moveBy(cursorMargin, 0);
    painter.drawText(textRect, m_text, TextAlignment::Left, Colors::Black);

    if (m_hasFocus) {
        // FIXME: get this from the font
        static const int fontWidth = 8;
        IntRect cursorRect = m_rect;
        cursorRect.setHeight(m_rect.height() - (2 * cursorMargin));
        cursorRect.setWidth(1);
        cursorRect.moveBy(cursorMargin + (ADS::max(0, m_cursorOffset) * fontWidth), (m_rect.height() - cursorRect.height()) / 2);
        painter.drawFilledRect(cursorRect, Colors::Black);
    }
}

void TextBox::onMouseUp(int key, int x, int y)
{
    m_cursorOffset = ADS::min(m_cursorOffset + 1, static_cast<int>(m_text.length()));
}

} // GUI