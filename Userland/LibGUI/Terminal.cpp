//
// Created by n3dry on 19.08.24.
//

#include "Terminal.h"
#include "Painter.h"
#include "String.h"

namespace GUI {

static int fontWidth()
{
    // FIXME: get this from the font
    return 8;
}

static int fontHeight()
{
    // FIXME: get this from the font
    return 10;
}

static const ADS::String terminalUserPath = "n3dry@OS> ";

Terminal::Terminal(Widget* parent)
    : Widget(parent)
{
}

void Terminal::onKeyDownEvent(KeyEvent& event)
{
    Widget::onKeyUpEvent(event);

    m_isCursorVisible = true;
    update();

    if (event.key() == Key::Key_Left) {
        return;
    }

    if (event.key() == Key::Key_Right) {
        return;
    }

    if (event.key() == Key::Key_Return) {
        // TODO Scroll View
        m_lines[++m_cursorRowPos].characters = terminalUserPath;
        m_cursorColumnPos = terminalUserPath.length();
        return;
    }

    if (event.key() == Key::Key_Backspace) {
        return;
    }

    if (event.key() == Key::Key_A && event.ctrl()) {
        return;
    }

    if (event.key() == Key::Key_Home) {
        return;
    }

    if (event.key() == Key::Key_End) {
        return;
    }

    Line& currentLine = m_lines[m_cursorRowPos];
    currentLine.characters.append(event.text());
    m_cursorColumnPos += event.text().length();
    update();
}

void Terminal::onResizeEvent(ResizeEvent& event)
{
    Widget::onResizeEvent(event);

    m_rows = event.size().height() / fontHeight();
    m_columns = event.size().width() / fontWidth();
    m_lines.resize(m_rows);
}

void Terminal::onPaintEvent(PaintEvent& event)
{
    Painter painter(*this);
    painter.setClipRect(event.rect());

    painter.drawFilledRect(event.rect(), Colors::Black);

    for (size_t row = 0; row < m_rows; ++row) {
        if (m_lines.size() <= row + m_rowScrollIndex)
            break;

        const Line& currentLine = m_lines[row + m_rowScrollIndex];
        const Rect lineRect { 0, static_cast<int>(row) * fontHeight(), static_cast<int>(m_columns) * fontWidth(), fontHeight() };
        painter.drawText(lineRect, currentLine.characters, Alignment::Left, Colors::Green);
    }

    if (m_isCursorVisible) {
        painter.drawFilledRect(cursorRect(), Colors::White);
    }
}

void Terminal::onFocusInEvent(FocusEvent& event)
{
    Widget::onFocusInEvent(event);
    m_blinkTimerId = startTimer(530);
    m_cursorColumnPos = terminalUserPath.length();
}

void Terminal::onFocusOutEvent(FocusEvent& event)
{
    Widget::onFocusOutEvent(event);
    killTimer(m_blinkTimerId);
    m_blinkTimerId = 0;
}

void Terminal::onTimerEvent(TimerEvent& event)
{
    CObject::onTimerEvent(event);
    m_isCursorVisible = !m_isCursorVisible;
    update();
}

void Terminal::onShowEvent(Event& event)
{
    Widget::onShowEvent(event);

    m_lines[0].characters.append(terminalUserPath);
    m_cursorColumnPos = terminalUserPath.length();
}

Size Terminal::preferredSizeHint() const
{
    return { 600, 400 };
}

Size Terminal::minSizeHint() const
{
    return { 35, 10 };
}

Rect Terminal::cursorRect() const
{
    Rect cursorRect;
    cursorRect.setX(m_lines[m_cursorRowPos].characters.length() * fontWidth());
    cursorRect.setY(m_cursorRowPos * fontHeight());
    cursorRect.setWidth(1);
    cursorRect.setHeight(fontHeight());
    return cursorRect;
}

}