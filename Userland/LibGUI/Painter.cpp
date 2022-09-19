//
// Created by n3dry on 17.09.22.
//

#include "Painter.h"
#include "Types.h"

namespace GUI {

Painter::Painter(Bitmap* targetBuffer)
    : m_targetBuffer(targetBuffer)
{
}

void Painter::drawRectangle(IntRect rect, GUI::Color color)
{
    for (int i = 0; i < rect.width(); ++i) {
        for (int j = 0; j < rect.height(); ++j) {
            m_targetBuffer->setPixel(rect.x() + i, rect.y() + j, color);
        }
    }
}

void Painter::drawQuad(int x, int y, int size, GUI::Color color)
{
    drawRectangle({ x, y, size, size }, color);
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

    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
    const int dx = ADS::abs(x1 - x0);
    const int dy = -ADS::abs(y1 - y0);
    const int sx = x0 < x1 ? 1 : -1;
    const int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;
    int x = x0;
    int y = y0;

    while (true) {
        m_targetBuffer->setPixel(x, y, color);
        if (x == x1 && y == y1)
            break;
        const int error2 = 2 * error;
        if (error2 >= dy) {
            if (x == x1)
                break;
            error += dy;
            x += sx;
        }
        if (error2 <= dx) {
            if (y == y1)
                break;
            error += dx;
            y += sy;
        }
    }
}
} // GUI