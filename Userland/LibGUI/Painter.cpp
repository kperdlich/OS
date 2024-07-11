//
// Created by n3dry on 17.09.22.
//

#include "Painter.h"
#include "ASCIIFont.h"
#include "DefaultFont8x10.h"
#include "Window.h"

namespace GUI {

static ASCIIFont s_defaultFont(DefaultFont::font, DefaultFont::firstCharacter, DefaultFont::lastCharacter, DefaultFont::fontWidth, DefaultFont::fontHeight);

Painter::Painter(Bitmap& bitmap)
    : m_targetBuffer(&bitmap)
    , m_clipRect(Rect { 0, 0, bitmap.size() })

{
}

Painter::Painter(Widget& widget)
    : m_targetBuffer(widget.window()->backBuffer())
    , m_clipRect(Rect { 0, 0, m_targetBuffer->size() })
    , m_relativeTranslationX(widget.windowRelativeRect().x())
    , m_relativeTranslationY(widget.windowRelativeRect().y())

{
}

void Painter::setClipRect(const Rect& clipRect)
{
    Rect translated = clipRect;
    translated.moveBy(m_relativeTranslationX, m_relativeTranslationY);
    m_clipRect = Rect { 0, 0, m_targetBuffer->size() }.intersectRect(translated);
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

void Painter::drawText(const Rect& rect, const ADS::String& text, Alignment alignment, GUI::Color color)
{
    if (text.empty())
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
    for (size_t i = 0; i < text.size(); ++i) {
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