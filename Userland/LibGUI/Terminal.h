//
// Created by n3dry on 19.08.24.
//

#pragma once

#include "Vector.h"
#include "Widget.h"

namespace GUI {

class Terminal : public Widget {
public:
    explicit Terminal(Widget* parent = nullptr);

    virtual const char* className() const override { return "Terminal"; }
    virtual bool acceptsFocus() const override { return true; };

    virtual Size preferredSizeHint() const override;
    virtual Size minSizeHint() const override;

protected:
    virtual void onKeyDownEvent(KeyEvent& event) override;
    virtual void onResizeEvent(ResizeEvent& event) override;
    virtual void onPaintEvent(PaintEvent& event) override;
    virtual void onFocusInEvent(FocusEvent& event) override;
    virtual void onFocusOutEvent(FocusEvent& event) override;
    virtual void onTimerEvent(TimerEvent& event) override;
    virtual void onShowEvent(Event& event) override;

private:
    Rect cursorRect() const;

    struct Line {
        ADS::String characters;
    };

private:
    ADS::Vector<Line> m_lines;
    size_t m_rowScrollIndex { 0 };
    size_t m_rows { 0 };
    size_t m_columns { 0 };
    size_t m_cursorRowPos { 0 };
    size_t m_cursorColumnPos { 0 };
    int m_blinkTimerId { 0 };
    bool m_isCursorVisible { false };
};

}