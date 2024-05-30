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

    virtual void onPaint() override;
    virtual void onMouseUp(int key, int x, int y) override;

    void setText(const ADS::String& text) { m_text = text; }
    ADS::String text() const { return m_text; }

private:
    ADS::String m_text;
    int m_cursorOffset { -1 };
};

} // GUI
