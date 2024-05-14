//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "Widget.h"
#include "Rect.h"
#include "Color.h"
#include "Types.h"

namespace GUI {

enum class TextAlignment {
    Left,
    Right,
    Center
};

class Painter {
public:
    explicit Painter(Widget* widget = nullptr);

    void drawRectangle(const IntRect& rect, GUI::Color color);
    void drawQuad(int x, int y, int size, GUI::Color color);
    void drawLine(int x0, int y0, int x1, int y1, GUI::Color color);
    void drawText(const IntRect& rect, const ADS::String& text, TextAlignment alignment, GUI::Color color);

private:
    int m_relativeTranslationX {};
    int m_relativeTranslationY {};
};

} // GUI
