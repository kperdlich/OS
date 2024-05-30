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
    : Widget(parent)
{
    setText(text);
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

    if (hasFocus()) {
        // FIXME: get this from the font
        static const int fontWidth = 8;
        IntRect cursorRect = m_rect;
        cursorRect.setHeight(m_rect.height() - (2 * cursorMargin));
        cursorRect.setWidth(1);
        cursorRect.moveBy(cursorMargin + (m_cursorOffset * fontWidth), (m_rect.height() - cursorRect.height()) / 2);
        painter.drawFilledRect(cursorRect, Colors::Black);
    }
}

void TextBox::onKeyDown(const KeyEvent& event)
{
    if (event.key() == Key::Left) {
        m_cursorOffset = ADS::max(m_cursorOffset - 1, 0);
    } else if (event.key() == Key::Right) {
        m_cursorOffset = ADS::min(m_cursorOffset + 1, static_cast<int>(m_text.length()));
    } else if (event.key() == Key::Backspace) {
        if (m_cursorOffset > 0 && m_text.length() > 0) {
            m_text.erase(--m_cursorOffset, 1);
        }
    } else {
        m_text.insert(m_cursorOffset++, event.text());
    }
}

void TextBox::setText(const ADS::String& text)
{
    m_text = text;
    m_cursorOffset = static_cast<int>(m_text.length());
}

} // GUI