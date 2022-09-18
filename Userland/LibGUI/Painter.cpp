//
// Created by n3dry on 17.09.22.
//

#include "Painter.h"
#include "Types.h"

namespace GUI {

// TODO Get target as bitmap to have size and width
Painter::Painter(Bitmap* targetBuffer)
    : m_targetBuffer(targetBuffer)
{
}

void Painter::drawRectangle(int x, int y, Rect size, GUI::Color color)
{
    for (int i = 0; i < size.width; ++i) {
        for (int j = 0; j < size.height; ++j) {
            m_targetBuffer->setPixel(x + i, y + j, color);
        }
    }
}
void Painter::drawQuad(int x, int y, int size, GUI::Color color)
{
    drawRectangle(x, y, { size, size }, color);
}
void Painter::drawLine(int x0, int y0, int x1, int y1, GUI::Color color)
{
    // Vertical Line
    if (x0 == x1) {
        const int startY = ADS::min(y0, y1);
        const int endY = ADS::max(y0, y1);

        for (int i = startY; i < endY; ++i) {
            m_targetBuffer->setPixel(x0, i, color);
        }

        return;
    }

    // Horizontal Line
    if (y0 == y1) {
        const int startX = ADS::min(x0, x1);
        const int endX = ADS::max(x0, x1);

        for (int i = startX; i < endX; ++i) {
            m_targetBuffer->setPixel(i, y0, color);
        }

        return;
    }

    // TODO Implement diagonal
    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
}
} // GUI