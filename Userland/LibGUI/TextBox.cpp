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

void TextBox::onPaintEvent(Event& event)
{
    static const int cursorMargin = 5;

    Painter painter(this);
    painter.drawFilledRect(m_windowRelativeRect, Colors::White);
    painter.drawRect(m_windowRelativeRect, Colors::Black);

    // Add TextBox margin
    IntRect textRect = m_windowRelativeRect;
    textRect.moveBy(cursorMargin, 0);
    textRect.setWidth(textRect.width() - cursorMargin);

    const int maxVisibleChars = textRect.width() / fontWidth();
    const int firstVisibleChar = ADS::max(m_cursorOffset - maxVisibleChars, 0);
    const int charsToDisplay = ADS::min(static_cast<int>(m_text.length()) - firstVisibleChar, maxVisibleChars);
    const ADS::String visibleText = m_text.substr(firstVisibleChar, charsToDisplay);

    painter.drawText(textRect, visibleText, TextAlignment::Left, Colors::Black);

    if (m_isCursorVisible && hasFocus()) {
        IntRect cursorRect = textRect;
        cursorRect.setHeight(textRect.height() - (2 * cursorMargin));
        cursorRect.setWidth(1);
        const int newCursorX = (m_cursorOffset - firstVisibleChar) * static_cast<int>(fontWidth());
        cursorRect.moveBy(newCursorX, (textRect.height() - cursorRect.height()) / 2);
        painter.drawFilledRect(cursorRect, Colors::Black);
    }
}

void TextBox::onKeyDownEvent(KeyEvent& event)
{
    if (event.key() == Key::Left) {
        m_cursorOffset = ADS::max(m_cursorOffset - 1, 0);
        return;
    }

    if (event.key() == Key::Right) {
        m_cursorOffset = ADS::min(m_cursorOffset + 1, static_cast<int>(m_text.length()));
        return;
    }

    if (event.key() == Key::Backspace) {
        if (m_cursorOffset > 0 && m_text.length() > 0)
            m_text.erase(--m_cursorOffset, 1);
        return;
    }

    m_text.insert(m_cursorOffset++, event.text());
}

void TextBox::setText(const ADS::String& text)
{
    m_text = text;
    m_cursorOffset = static_cast<int>(m_text.length());
}

void TextBox::onFocusInEvent(FocusEvent& event)
{
    m_isCursorVisible = true;
    m_blinkTimerId = startTimer(530);
}

void TextBox::onFocusOutEvent(FocusEvent& event)
{
    killTimer(m_blinkTimerId);
    m_blinkTimerId = 0;
    m_isCursorVisible = false;
}

void TextBox::onTimerEvent(TimerEvent& event)
{
    m_isCursorVisible = !m_isCursorVisible;
}

size_t TextBox::fontWidth()
{
    // FIXME: get this from the font
    return 8;
}

} // GUI