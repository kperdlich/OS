//
// Created by Kevin on 29.05.24.
//

#pragma once

#include "Size.h"
#include "Types.h"
#include "Widget.h"

namespace GUI {

class Cursor final {
public:
    Cursor() = default;
    Cursor(int position)
        : m_position(position)
    {
    }
    Cursor(int selectionStart, int selectionEnd)
        : m_selectionStart(selectionStart)
        , m_selectionEnd(selectionEnd)
    {
    }

    void moveCursorLeft()
    {
        --m_position;
    }

    void moveCursorRight()
    {
        ++m_position;
    }

    void reduceSelectionLeft()
    {
        ++m_selectionStart;
    }

    void expandSelectionLeft()
    {
        --m_selectionStart;
    }

    void expandSelectionRight()
    {
        ++m_selectionEnd;
    }

    void reduceSelectionRight()
    {
        --m_selectionEnd;
    }

    void clearSelection()
    {
        m_selectionEnd = 0;
        m_selectionStart = 0;
    }

    // Inclusive
    [[nodiscard]] int selectionStart() const { return m_selectionStart; }

    // Exclusive
    [[nodiscard]] int selectionEnd() const { return m_selectionEnd; }

    void setSelectionStart(int value)
    {
        m_selectionStart = value;
    }

    void setSelectionEnd(int value)
    {
        m_selectionEnd = value;
    }

    [[nodiscard]] bool hasSelection() const { return m_selectionStart != m_selectionEnd; }
    [[nodiscard]] bool selectionIncludes(int index) const { return hasSelection() && index >= m_selectionStart && index < m_selectionEnd; }

    void setPosition(int value)
    {
        m_position = value;
    };

    [[nodiscard]] int position() const { return m_position; };

private:
    int m_position { 0 };
    int m_selectionStart { 0 };
    int m_selectionEnd { 0 };
};

class TextBox : public Widget {
public:
    explicit TextBox(Widget* parent = nullptr);
    TextBox(const ADS::String& text, Widget* parent = nullptr);

    virtual const char* className() const override { return "TextBox"; }

    void setText(const ADS::String& text);
    ADS::String text() const { return m_text; }

    void selectAll();

    virtual bool acceptsFocus() const { return true; }
    virtual Size preferredSizeHint() const override;
    virtual Size minSizeHint() const override;

protected:
    virtual void onPaintEvent(PaintEvent& event) override;
    virtual void onTimerEvent(TimerEvent& event) override;
    virtual void onKeyDownEvent(KeyEvent& event) override;
    virtual void onFocusInEvent(FocusEvent& event) override;
    virtual void onFocusOutEvent(FocusEvent& event) override;
    virtual void onResizeEvent(ResizeEvent& event) override;
    virtual void onMouseDownEvent(MouseEvent& event) override;
    virtual void onMouseMoveEvent(MouseEvent& event) override;
    virtual void onMouseUpEvent(MouseEvent& event) override;

    void scrollCursorIntoView();

private:
    static int fontWidth();
    static int margin();

    int maxVisibleChars() const;

    Rect innerRect() const;
    Rect cursorRect() const;
    Rect selectionRect() const;

    void handleKeyLeft(KeyEvent& event);
    void handleKeyRight(KeyEvent& event);
    void handleKeyBackspace(KeyEvent& event);
    void removeSelectedText();
    void cleanup();

private:
    ADS::String m_text;
    Cursor m_cursor;
    int m_scrollOffset { 0 };
    int m_blinkTimerId { 0 };
    bool m_isCursorVisible { false };
    bool m_inSelectionMode { false };
};

} // GUI
