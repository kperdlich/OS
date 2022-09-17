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
    explicit Painter(Bitmap* target);

    void drawRectangle(int x, int y, Rect size, Gui::Color color);
    void drawQuad(int x, int y, int size, Gui::Color color);

private:
    Bitmap* m_targetBuffer = nullptr;
};

} // GUI
