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

void TextBox::setText(const ADS::String& text)
{
    m_text = text;
    m_cursor.setPosition(static_cast<int>(m_text.length()));
    m_cursor.clearSelection();
    m_scrollOffset = 0;
    if (hasFocus())
        scrollCursorIntoView();
}

Rect TextBox::innerRect() const
{
    const int cursorMargin = static_cast<int>(margin());
    Rect textRect = rect();
    textRect.moveBy(cursorMargin, 0);
    textRect.setWidth(textRect.width() - cursorMargin);
    return textRect;
}

Rect TextBox::cursorRect() const
{
    Rect cursorRect = innerRect();
    cursorRect.setHeight(innerRect().height() - (2 * margin()));
    cursorRect.setWidth(1);
    const int newCursorX = (m_cursor.position() - m_scrollOffset) * fontWidth();
    const int newCursorY = (innerRect().height() - cursorRect.height()) / 2;
    cursorRect.moveBy(newCursorX, newCursorY);

    return cursorRect;
}

Rect TextBox::selectionRect() const
{
    const int firstVisibleSelectedChar = ADS::max(m_cursor.selectionStart(), m_scrollOffset);
    const int lastVisibleSelectedChar = ADS::min(m_cursor.selectionEnd(), m_scrollOffset + maxVisibleChars());
    const int selectedChars = lastVisibleSelectedChar - firstVisibleSelectedChar;

    Rect selectionRect = innerRect();
    selectionRect.setHeight(innerRect().height() - (2 * margin()));
    selectionRect.setWidth(selectedChars * fontWidth());
    const int selectionX = ADS::max((m_cursor.selectionStart() - m_scrollOffset), 0) * fontWidth();
    const int selectionY = (innerRect().height() - selectionRect.height()) / 2;
    selectionRect.moveBy(selectionX, selectionY);
    return selectionRect;
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

int TextBox::maxVisibleChars() const
{
    return innerRect().width() / fontWidth();
}

void TextBox::scrollCursorIntoView()
{
    if (m_cursor.position() >= m_scrollOffset + maxVisibleChars()) {
        m_scrollOffset = m_cursor.position() - maxVisibleChars();
    } else if (m_cursor.position() < m_scrollOffset) {
        m_scrollOffset = m_cursor.position();
    }

#if 0
    std::cout << "cursor position " << m_cursor.position() << " m_scrollOffset " << m_scrollOffset << std::endl;
#endif
}

void TextBox::onPaintEvent(PaintEvent& event)
{
    Painter painter(this);
    painter.setClipRect(event.rect());

    painter.drawFilledRect(rect(), Colors::White);
    painter.drawRect(rect(), Colors::Black);

    if (m_cursor.hasSelection()) {
        painter.drawFilledRect(selectionRect(), Colors::Blue);
    }

    const ADS::String visibleText = m_text.substr(m_scrollOffset, ADS::min(static_cast<int>(m_text.length()) - m_scrollOffset, maxVisibleChars()));
    for (size_t i = 0; i < visibleText.length(); ++i) {
        Rect charRect = innerRect();
        charRect.moveBy(static_cast<int>(i) * fontWidth(), 0);
        // FIXME: avoid creating a string for char.
        const ADS::String character { visibleText[i] };
        painter.drawText(charRect, character, Alignment::Left, m_cursor.selectionIncludes(i + m_scrollOffset) ? Colors::White : Colors::Black);
    }

    if (m_isCursorVisible && hasFocus()) {
        painter.drawFilledRect(cursorRect(), Colors::Black);
    }
}

void TextBox::onKeyDownEvent(KeyEvent& event)
{
    m_isCursorVisible = true;

    if (event.key() == Key::Key_Left) {
        handleKeyLeft(event);
        return;
    }

    if (event.key() == Key::Key_Right) {
        handleKeyRight(event);
        return;
    }

    if (event.key() == Key::Key_Backspace) {
        handleKeyBackspace(event);
        return;
    }

    if (event.key() == Key::Key_A && event.ctrl()) {
        selectAll();
        return;
    }

    if (event.key() == Key::Key_Home) {
        m_cursor.setPosition(0);
        m_cursor.clearSelection();
        scrollCursorIntoView();
        return;
    }

    if (event.key() == Key::Key_End) {
        m_cursor.setPosition(static_cast<int>(m_text.length()));
        m_cursor.clearSelection();
        scrollCursorIntoView();
        return;
    }

    if (event.text().length() > 0) {
        if (m_cursor.hasSelection()) {
            m_text.erase(m_cursor.selectionStart(), m_cursor.selectionEnd() - m_cursor.selectionStart());
            m_cursor.setPosition(m_cursor.selectionStart());
            m_cursor.clearSelection();
        }
        m_text.insert(m_cursor.position(), event.text());
        m_cursor.moveCursorRight();
        scrollCursorIntoView();
    }
}

void TextBox::onFocusInEvent(FocusEvent&)
{
    m_isCursorVisible = true;
    m_blinkTimerId = startTimer(530);
}

void TextBox::onFocusOutEvent(FocusEvent&)
{
    cleanup();
}

void TextBox::onTimerEvent(TimerEvent& event)
{
    Widget::onTimerEvent(event);
    m_isCursorVisible = !m_isCursorVisible;
}

void TextBox::onResizeEvent(ResizeEvent& event)
{
    Widget::onResizeEvent(event);
}

void TextBox::onMouseDownEvent(MouseEvent& event)
{
    const int newCursorPos = ADS::max((event.x() / fontWidth()) - 1, 0);
    m_cursor.setPosition(ADS::min(m_scrollOffset + newCursorPos, static_cast<int>(m_text.length())));
    m_cursor.clearSelection();
    m_isCursorVisible = true;
    m_inSelectionMode = true;
    grabMouse();
}

void TextBox::onMouseMoveEvent(MouseEvent& event)
{
    if (!m_inSelectionMode)
        return;

    m_isCursorVisible = true;
    const int cursorPos = event.x() / fontWidth();
    const int newCursorPos = ADS::clamp(m_scrollOffset + cursorPos, 0, static_cast<int>(m_text.length()));

    if (m_cursor.hasSelection()) {
        if (m_cursor.position() == m_cursor.selectionStart())
            m_cursor.setSelectionStart(newCursorPos);
        else
            m_cursor.setSelectionEnd(newCursorPos);

        const int selectionStart = m_cursor.selectionStart();
        const int selectionEnd = m_cursor.selectionEnd();
        m_cursor.setSelectionStart(ADS::min(selectionStart, selectionEnd));
        m_cursor.setSelectionEnd(ADS::max(selectionStart, selectionEnd));
        m_cursor.setPosition(newCursorPos);
    } else {
        m_cursor.setSelectionStart(ADS::min(newCursorPos, m_cursor.position()));
        m_cursor.setSelectionEnd(ADS::max(newCursorPos, m_cursor.position()));
        m_cursor.setPosition(newCursorPos);
    }
    scrollCursorIntoView();

#if 0
    std::cout << "cursor " << m_cursor.position() << " m_selectionRange: start: " << m_cursor.selectionStart() << " end: " << m_cursor.selectionEnd() << std::endl;
#endif
}

void TextBox::onMouseUpEvent(MouseEvent& event)
{
    m_inSelectionMode = false;
    releaseMouse();
}

void TextBox::removeSelectedText()
{
    ASSERT(m_cursor.hasSelection());
    const int newCursorPos = m_cursor.selectionStart();
    m_text.erase(m_cursor.selectionStart(), m_cursor.selectionEnd() - m_cursor.selectionStart());
    m_cursor.setPosition(newCursorPos);
    m_cursor.clearSelection();
    scrollCursorIntoView();
}

void TextBox::selectAll()
{
    if (m_text.empty())
        return;

    const int textLength = static_cast<int>(m_text.length());
    m_cursor.setPosition(textLength);
    m_cursor.setSelectionStart(0);
    m_cursor.setSelectionEnd(textLength);
    scrollCursorIntoView();
}

void TextBox::handleKeyLeft(KeyEvent& event)
{
    if (event.shift()) {
        if (m_cursor.position() > 0) {
            if (m_cursor.hasSelection()) {
                if (m_cursor.selectionStart() == m_cursor.position()) {
                    m_cursor.expandSelectionLeft();
                } else {
                    m_cursor.reduceSelectionRight();
                }
            } else {
                m_cursor.setSelectionStart(m_cursor.position() - 1);
                m_cursor.setSelectionEnd(m_cursor.position());
            }
            m_cursor.moveCursorLeft();
        }
    } else {
        if (m_cursor.hasSelection()) {
            m_cursor.setPosition(m_cursor.selectionStart());
            m_cursor.clearSelection();
        } else if (m_cursor.position() > 0) {
            m_cursor.moveCursorLeft();
        }
    }
    scrollCursorIntoView();
}

void TextBox::handleKeyRight(KeyEvent& event)
{
    if (event.shift()) {
        if (m_cursor.position() < m_text.length()) {
            if (m_cursor.hasSelection()) {
                if (m_cursor.selectionEnd() == m_cursor.position()) {
                    m_cursor.expandSelectionRight();
                } else {
                    m_cursor.reduceSelectionLeft();
                }
            } else {
                m_cursor.setSelectionStart(m_cursor.position());
                m_cursor.setSelectionEnd(m_cursor.position() + 1);
            }
            m_cursor.moveCursorRight();
        }
    } else {
        if (m_cursor.hasSelection()) {
            m_cursor.setPosition(m_cursor.selectionEnd());
            m_cursor.clearSelection();
        } else if (m_cursor.position() < m_text.length()) {
            m_cursor.moveCursorRight();
        }
    }
    scrollCursorIntoView();
}

void TextBox::handleKeyBackspace(KeyEvent& event)
{
    if (m_cursor.hasSelection()) {
        removeSelectedText();
    } else if (m_cursor.position() > 0 && m_text.length() > 0) {
        m_cursor.moveCursorLeft();
        m_text.erase(m_cursor.position(), 1);
        m_scrollOffset = ADS::max(m_scrollOffset - 1, 0);
    }
}

void TextBox::cleanup()
{
    killTimer(m_blinkTimerId);
    m_blinkTimerId = 0;
    m_isCursorVisible = false;
    m_inSelectionMode = false;
    m_cursor.clearSelection();
}

Size TextBox::preferredSizeHint() const
{
    if (m_text.empty())
        return minSizeHint();

    Size size = minSizeHint();
    size.setWidth(ADS::max((static_cast<int>(m_text.length()) * fontWidth()) + (2 * margin()), size.width()));
    return size;
}

Size TextBox::minSizeHint() const
{
    return { 133, 22 };
}

} // GUI