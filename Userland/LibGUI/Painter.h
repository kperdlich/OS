//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "Widget.h"
#include "Rect.h"
#include "Color.h"

namespace GUI {

class Painter {
public:
    explicit Painter(Widget* widget = nullptr);

    void drawRectangle(const IntRect& rect, GUI::Color color);
    void drawQuad(int x, int y, int size, GUI::Color color);
    void drawLine(int x0, int y0, int x1, int y1, GUI::Color color);

private:
    int m_relativeTranslationX {};
    int m_relativeTranslationY {};
};

} // GUI
