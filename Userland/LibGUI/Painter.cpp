//
// Created by n3dry on 17.09.22.
//

#include "Painter.h"

namespace GUI {

// TODO Get target as bitmap to have size and width
Painter::Painter(Bitmap* targetBuffer)
    : m_targetBuffer(targetBuffer)
{

}

void Painter::drawRectangle(int x, int y, Rect size, Gui::Color color)
{
    for (int i = 0; i < size.width; ++i) {
        for (int j = 0; j < size.height; ++j) {
            m_targetBuffer->setPixel(x + i, y + j, color);
        }
    }
}
void Painter::drawQuad(int x, int y, int size, Gui::Color color)
{
    drawRectangle(x, y, {size, size}, color);
}
} // GUI