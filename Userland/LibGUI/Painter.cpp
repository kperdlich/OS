//
// Created by n3dry on 17.09.22.
//

#include "Painter.h"
#include "ASCIIFont.h"
#include "DefaultFont8x10.h"
#include "Window.h"
#include <cmath>

namespace GUI {

static ASCIIFont s_defaultFont(DefaultFont::font, DefaultFont::firstCharacter, DefaultFont::lastCharacter, DefaultFont::fontWidth, DefaultFont::fontHeight);

Painter::Painter(Bitmap& bitmap)
    : m_targetBuffer(&bitmap)
    , m_clipRect(Rect { 0, 0, bitmap.size() })

{
}

Painter::Painter(Widget& widget)
    : m_targetBuffer(widget.window()->backBuffer())
    , m_clipRect(Rect { {}, m_targetBuffer->size() })
    , m_relativeTranslationX(widget.windowRelativeRect().x())
    , m_relativeTranslationY(widget.windowRelativeRect().y())

{
}

void Painter::setClipRect(const Rect& clipRect)
{
    Rect translated = clipRect;
    translated.moveBy(m_relativeTranslationX, m_relativeTranslationY);
    m_clipRect = Rect { {}, m_targetBuffer->size() }.intersectRect(translated);
}

void Painter::drawFilledRect(const Rect& rect, GUI::Color color)
{
#if 0
    const auto start = std::chrono::system_clock::now();
#endif

    Rect clippedAndTranslatedRect = rect;
    clippedAndTranslatedRect.moveBy(m_relativeTranslationX, m_relativeTranslationY);
    clippedAndTranslatedRect.intersect(m_clipRect);

    if (clippedAndTranslatedRect.isEmpty())
        return;

    const int startX = clippedAndTranslatedRect.x();
    const int startY = clippedAndTranslatedRect.y();
    const int width = clippedAndTranslatedRect.width();
    const int height = clippedAndTranslatedRect.height();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            m_targetBuffer->setPixel(startX + x, startY + y, color);
        }
    }

#if 0
    const auto passed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
    std::cout << "[Painter::drawFilledRect] took " << passed.count() << " ms" << std::endl;
#endif
}

void Painter::drawRect(const Rect& rect, GUI::Color color)
{
    Rect translated = rect;
    translated.moveBy(m_relativeTranslationX, m_relativeTranslationY);

    const auto clippedRect = m_clipRect.intersectRect(translated);
    for (int y = 0; y < clippedRect.height(); ++y) {
        if (y == 0 || y == clippedRect.height() - 1) {
            for (int x = 0; x < clippedRect.width(); ++x) {
                m_targetBuffer->setPixel(clippedRect.x() + x, clippedRect.y() + y, color);
            }
        } else {
            if (clippedRect.contains(clippedRect.x(), clippedRect.y() + y))
                m_targetBuffer->setPixel(clippedRect.x(), clippedRect.y() + y, color);
            if (clippedRect.contains(clippedRect.x() + clippedRect.width() - 1, clippedRect.y() + y))
                m_targetBuffer->setPixel(clippedRect.x() + clippedRect.width() - 1, clippedRect.y() + y, color);
        }
    }
}

void Painter::drawFilledQuad(int x, int y, int size, GUI::Color color)
{
    drawFilledRect({ x, y, size, size }, color);
}

void Painter::drawLine(const Point& x, const Point& y, GUI::Color color)
{
    drawLine(x.x(), x.y(), y.x(), y.y(), color);
}

void Painter::drawLine(int x0, int y0, int x1, int y1, GUI::Color color)
{
    x0 += m_relativeTranslationX;
    y0 += m_relativeTranslationY;
    x1 += m_relativeTranslationX;
    y1 += m_relativeTranslationY;

    const Rect clippedRect = m_clipRect;

    // Vertical Line
    if (x0 == x1) {
        const int startY = ADS::min(y0, y1);
        const int endY = ADS::max(y0, y1);
        for (int i = startY; i <= endY; ++i) {
            if (clippedRect.contains(x0, i))
                m_targetBuffer->setPixel(x0, i, color);
        }
        return;
    }

    // Horizontal Line
    if (y0 == y1) {
        const int startX = ADS::min(x0, x1);
        const int endX = ADS::max(x0, x1);
        for (int i = startX; i <= endX; ++i) {
            if (clippedRect.contains(i, y0))
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
        if (clippedRect.contains(x, y))
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

static void forEachEllipsePoint(const Rect& rect, ADS::Function<void(const Point&)>&& callback)
{
    // Implementation of the Midpoint ellipse drawing algorithm.

    // Radii along the x and y axes
    const int rx = rect.width() / 2;
    const int ry = rect.height() / 2;

    // Variables for the algorithm
    int x = 0;
    int y = ry;
    const int rx2 = rx * rx;
    const int ry2 = ry * ry;
    const int twoRx2 = 2 * rx2;
    const int twoRy2 = 2 * ry2;
    int px = 0;
    int py = twoRx2 * y;

    // Region 1
    int p = static_cast<int>(round(ry2 - (rx2 * ry) + (0.25 * rx2)));
    while (px < py) {
        callback({ x, y });

        x++;
        px += twoRy2;
        if (p < 0) {
            p += ry2 + px;
        } else {
            y--;
            py -= twoRx2;
            p += ry2 + px - py;
        }
    }

    // Region 2
    p = static_cast<int>(round(ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2));
    while (y >= 0) {
        callback({ x, y });

        y--;
        py -= twoRx2;
        if (p > 0) {
            p += rx2 - py;
        } else {
            x++;
            px += twoRy2;
            p += rx2 - py + px;
        }
    }
}

void Painter::drawEllipse(const Rect& rect, GUI::Color color)
{
    Rect translated = rect;
    translated.moveBy(m_relativeTranslationX, m_relativeTranslationY);
    const Rect clippedRect = m_clipRect.intersectRect(translated);

    const Point center = clippedRect.center();
    forEachEllipsePoint(clippedRect, [this, center, color](const Point& point) {
        m_targetBuffer->setPixel(center.x() + point.x(), center.y() + point.y(), color);
        m_targetBuffer->setPixel(center.x() - point.x(), center.y() + point.y(), color);
        m_targetBuffer->setPixel(center.x() + point.x(), center.y() - point.y(), color);
        m_targetBuffer->setPixel(center.x() - point.x(), center.y() - point.y(), color);
    });
}

void Painter::drawFilledEllipse(const Rect& rect, GUI::Color color)
{
    Rect translated = rect;
    translated.moveBy(m_relativeTranslationX, m_relativeTranslationY);
    const Rect clippedRect = m_clipRect.intersectRect(translated);

    auto drawHorizontalLine = [this](int xStart, int xEnd, int y, const Color& color) {
        for (int x = xStart; x <= xEnd; x++) {
            m_targetBuffer->setPixel(x, y, color);
        }
    };

    const Point center = clippedRect.center();
    forEachEllipsePoint(clippedRect, [center, color, drawHorizontalLine](const Point& point) {
        drawHorizontalLine(center.x() - point.x(), center.x() + point.x(), center.y() + point.y(), color);
        drawHorizontalLine(center.x() - point.x(), center.x() + point.x(), center.y() - point.y(), color);
    });
}

void Painter::drawText(const Rect& rect, const ADS::String& text, Alignment alignment, GUI::Color color)
{
    if (text.isEmpty())
        return;

    Rect translated = rect;
    translated.moveBy(m_relativeTranslationX, m_relativeTranslationY);

    // FIXME: add some margin/padding
    switch (alignment) {
    case Alignment::Right:
        translated.moveBy(rect.width() - (s_defaultFont.width() * text.length()), 0);
        break;
    case Alignment::Center:
        translated.moveBy((rect.width() - (s_defaultFont.width() * text.length())) / 2, 0);
        break;
    }

    // Center horizontal by default
    translated.moveBy(0, (rect.height() - s_defaultFont.height()) / 2);

    const auto clippedRect = m_clipRect.intersectRect(translated);
    for (size_t i = 0; i < text.length(); ++i) {
        const char asciiChar = text[i];
        if (asciiChar == ' ') {
            continue;
        }

        const char* font = s_defaultFont.GetAsciiBitmapForChar(asciiChar);
        if (!font) {
            std::cerr << "[Painter::drawText] Ascii char " << asciiChar << " not in font!" << std::endl;
            return;
        }

        for (int x = 0; x < s_defaultFont.width(); ++x) {
            for (int y = 0; y < s_defaultFont.height(); ++y) {
                const size_t index = (y * s_defaultFont.width()) + x;
                if (font[index] == '#') {
                    const int xPos = translated.x() + x + (i * s_defaultFont.width());
                    const int yPos = translated.y() + y;
                    if (clippedRect.contains(xPos, yPos))
                        m_targetBuffer->setPixel(xPos, yPos, color);
                }
            }
        }
    }
}

void Painter::drawCharacterBitmap(const Point& point, const CharacterBitmap& bitmap, Color color)
{
    Rect translated = { point.x(), point.y(), bitmap.width(), bitmap.height() };
    translated.moveBy(m_relativeTranslationX, m_relativeTranslationY);

    const char* data = bitmap.data();
    const Rect clippedRect = m_clipRect.intersectRect(translated);

    for (int y = 0; y < clippedRect.height(); ++y) {
        for (int x = 0; x < clippedRect.width(); ++x) {
            const int index = (y * clippedRect.width()) + x;
            if (data[index] == '#') {
                const int xPos = translated.x() + x;
                const int yPos = translated.y() + y;
                if (clippedRect.contains(xPos, yPos))
                    m_targetBuffer->setPixel(xPos, yPos, color);
            }
        }
    }
}

void Painter::blit(Point point, Bitmap& source)
{
    ASSERT(source.format() == m_targetBuffer->format());

    const Rect sourceRect = { point, source.size() };
    const Rect targetRect = { 0, 0, m_targetBuffer->size() };
    const Rect clippedRect = targetRect.intersectRect(sourceRect);
    if (clippedRect.isEmpty())
        return;

    const int byteDensity = m_targetBuffer->byteDensity();

    const int sourceStartX = clippedRect.x() - point.x();
    const int sourceStartY = clippedRect.y() - point.y();
    char* src = source.data() + (sourceStartY * source.width() + sourceStartX) * byteDensity;

    const int dstStartX = clippedRect.x();
    const int dstStartY = clippedRect.y();
    char* dst = m_targetBuffer->data() + (dstStartY * m_targetBuffer->width() + dstStartX) * byteDensity;

    const int rowBytes = clippedRect.width() * byteDensity;

    for (int row = 0; row < clippedRect.height(); ++row) {
        ADS::memcpy(dst, src, rowBytes);
        src += source.width() * byteDensity;
        dst += m_targetBuffer->width() * byteDensity;
    }
}

} // GUI