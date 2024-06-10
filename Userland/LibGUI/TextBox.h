//
// Created by Kevin on 29.05.24.
//

#pragma once

#include "Size.h"
#include "Types.h"
#include "Widget.h"

namespace GUI {

class TextBox : public Widget {
public:
    explicit TextBox(Widget* parent = nullptr);
    TextBox(const ADS::String& text, Widget* parent = nullptr);

    void setText(const ADS::String& text);
    ADS::String text() const { return m_text; }

    virtual const char* name() const override { return "TextBox"; }

protected:
    virtual void onPaintEvent(Event& event) override;
    virtual void onTimerEvent(TimerEvent& event) override;
    virtual void onKeyDownEvent(KeyEvent& event) override;
    virtual void onFocusInEvent(FocusEvent& event) override;
    virtual void onFocusOutEvent(FocusEvent& event) override;

private:
    static size_t fontWidth() ;

private:
    ADS::String m_text;
    int m_cursorOffset { 0 };
    int m_blinkTimerId { 0 };
    bool m_isCursorVisible { false };
};

} // GUI
