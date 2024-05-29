//
// Created by n3dry on 17.09.22.
//

#include "Painter.h"
#include "ASCIIFont.h"
#include "DefaultFont8x10.h"
#include "Screen.h"
#include "Window.h"

namespace GUI {

static ASCIIFont s_defaultFont(DefaultFont::font, DefaultFont::firstCharacter, DefaultFont::lastCharacter, DefaultFont::fontWidth, DefaultFont::fontHeight);

Painter::Painter(Widget* widget)
{
    if (!widget)
        return;

    m_relativeTranslationX = widget->rect().x();
    m_relativeTranslationY = widget->rect().y();

    if (const Window* window = widget->window()) {
        m_relativeTranslationX += window->rect().x();
        m_relativeTranslationY += window->rect().y();
    }
}

void Painter::drawFilledRect(const IntRect& rect, GUI::Color color)
{
    IntRect translated = rect;
    translated.moveBy(m_relativeTranslationX, m_relativeTranslationY);

    const auto clippedRect = IntRect { 0, 0, Screen::the().width(), Screen::the().height() }.clip(translated);
    for (int y = 0; y < clippedRect.height(); ++y) {
        for (int x = 0; x < clippedRect.width(); ++x) {
            Screen::the().setPixel(clippedRect.x() + x, clippedRect.y() + y, color);
        }
    }
}

void Painter::drawRect(const IntRect& rect, GUI::Color color)
{
    IntRect translated = rect;
    translated.moveBy(m_relativeTranslationX, m_relativeTranslationY);

    const auto clippedRect = IntRect { 0, 0, Screen::the().width(), Screen::the().height() }.clip(translated);
    for (int y = 0; y < clippedRect.height(); ++y) {
        if (y == 0 || y == clippedRect.height() - 1) {
            for (int x = 0; x < clippedRect.width(); ++x) {
                Screen::the().setPixel(clippedRect.x() + x, clippedRect.y() + y, color);
            }
        } else {
            // FIXME: Fix clipping here
            Screen::the().setPixel(clippedRect.x(), clippedRect.y() + y, color);
            Screen::the().setPixel(clippedRect.x() + clippedRect.width() - 1, clippedRect.y() + y, color);
        }
    }
}

void Painter::drawFilledQuad(int x, int y, int size, GUI::Color color)
{
    drawFilledRect({ x, y, size, size }, color);
}

void Painter::drawLine(int x0, int y0, int x1, int y1, GUI::Color color)
{
    // Vertical Line
    if (x0 == x1) {
        const int startY = ADS::min(y0, y1);
        const int endY = ADS::max(y0, y1);
        for (int i = startY; i < endY; ++i) {
            Screen::the().setPixel(x0, i, color);
        }
        return;
    }

    // Horizontal Line
    if (y0 == y1) {
        const int startX = ADS::min(x0, x1);
        const int endX = ADS::max(x0, x1);
        for (int i = startX; i < endX; ++i) {
            Screen::the().setPixel(i, y0, color);
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
        Screen::the().setPixel(x, y, color);
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

void Painter::drawText(const IntRect& rect, const ADS::String& text, TextAlignment alignment, GUI::Color color)
{
    if (text.empty())
        return;

    IntRect translated = rect;
    translated.moveBy(m_relativeTranslationX, m_relativeTranslationY);

    // FIXME: add some margin/padding
    switch (alignment) {
    case TextAlignment::Right:
        translated.moveBy(rect.width() - (s_defaultFont.width() * text.length()), 0);
        break;
    case TextAlignment::Center:
        translated.moveBy((rect.width() - (s_defaultFont.width() * text.length())) / 2, 0);
        break;
    }

    // Center horizontal by default
    translated.moveBy(0, (rect.height() - s_defaultFont.height()) / 2);

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
                    // FIXME: Fix clipping here
                    const int xPos = translated.x() + x + (i * s_defaultFont.width());
                    const int yPos = translated.y() + y;
                    Screen::the().setPixel(xPos, yPos, color);
                }
            }
        }
    }
}

void Painter::drawCharacterBitmap(const IntPoint& point, const CharacterBitmap& bitmap, Color color)
{
    IntRect translated = { point.x(), point.y(), bitmap.width(), bitmap.height() };
    translated.moveBy(m_relativeTranslationX, m_relativeTranslationY);

    const char* data = bitmap.data();

    const IntRect clippedRect = IntRect { 0, 0, Screen::the().width(), Screen::the().height() }.clip(translated);

    for (int y = 0; y < clippedRect.height(); ++y) {
        for (int x = 0; x < clippedRect.width(); ++x) {
            const int index = (y * clippedRect.width()) + x;
            if (data[index] == '#') {
                // FIXME: Fix clipping here
                Screen::the().setPixel(translated.x() + x, translated.y() + y, color);
            }
        }
    }
}

} // GUI