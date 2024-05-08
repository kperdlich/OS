//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "Bitmap.h"
#include "Rect.h"
#include "Color.h"

namespace GUI {

class Painter {
public:
    explicit Painter(Bitmap& target);

    void drawRectangle(IntRect rect, GUI::Color color);
    void drawQuad(int x, int y, int size, GUI::Color color);
    void drawLine(int x0, int y0, int x1, int y1, GUI::Color color);

private:
    Bitmap& m_targetBuffer;
};

} // GUI
