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
    Painter painter(this);
    painter.drawFilledRect(m_windowRelativeRect, Colors::White);
    painter.drawRect(m_windowRelativeRect, Colors::Black);

    const int maxVisibleChars = innerRect().width() / fontWidth();
    const ADS::String visibleText = m_text.substr(m_scrollOffset, ADS::min(static_cast<int>(m_text.length()) - m_scrollOffset, maxVisibleChars));
    painter.drawText(innerRect(), visibleText, TextAlignment::Left, Colors::Black);

    if (m_isCursorVisible && hasFocus()) {
        painter.drawFilledRect(cursorRect(), Colors::Black);
    }
}

void TextBox::onKeyDownEvent(KeyEvent& event)
{
    if (event.key() == Key::Left) {
        m_cursorPosition = ADS::max(m_cursorPosition - 1, 0);
        scrollCursorIntoView();
        return;
    }

    if (event.key() == Key::Right) {
        m_cursorPosition = ADS::min(m_cursorPosition + 1, static_cast<int>(m_text.length()));
        scrollCursorIntoView();
        return;
    }

    if (event.key() == Key::Backspace) {
        if (m_cursorPosition > 0 && m_text.length() > 0) {
            m_text.erase(--m_cursorPosition, 1);
            scrollCursorIntoView();
        }
        return;
    }

    m_text.insert(m_cursorPosition++, event.text());
    scrollCursorIntoView();
}

void TextBox::setText(const ADS::String& text)
{
    m_text = text;
    m_cursorPosition = static_cast<int>(m_text.length());
    if (hasFocus())
        scrollCursorIntoView();
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

int TextBox::fontWidth()
{
    // FIXME: get this from the font
    return 8;
}

int TextBox::margin()
{
    return 5;
}

void TextBox::scrollCursorIntoView()
{
    const int maxVisibleChars = innerRect().width() / fontWidth();

    if (m_cursorPosition >= m_scrollOffset + maxVisibleChars) {
        m_scrollOffset = m_cursorPosition - maxVisibleChars;
    } else if (m_cursorPosition < m_scrollOffset) {
        m_scrollOffset = m_cursorPosition;
    }

#if 0
    std::cout << "m_cursorPosition " << m_cursorPosition << std::endl;
    std::cout << "m_scrollOffset " << m_scrollOffset << std::endl;
#endif
}

Rect TextBox::innerRect() const
{
    const int cursorMargin = static_cast<int>(margin());
    Rect textRect = m_windowRelativeRect;
    textRect.moveBy(cursorMargin, 0);
    textRect.setWidth(textRect.width() - cursorMargin);
    return textRect;
}

Rect TextBox::cursorRect() const
{
    Rect cursorRect = innerRect();
    cursorRect.setHeight(innerRect().height() - (2 * margin()));
    cursorRect.setWidth(1);
    const int newCursorX = (m_cursorPosition - m_scrollOffset) * fontWidth();
    const int newCursorY = (innerRect().height() - cursorRect.height()) / 2;
    cursorRect.moveBy(newCursorX, newCursorY);

    return cursorRect;
}

void TextBox::onResizeEvent(ResizeEvent&)
{
    scrollCursorIntoView();
}

} // GUI