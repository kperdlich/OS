//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "Alignment.h"
#include "Bitmap.h"
#include "CharacterBitmap.h"
#include "Color.h"
#include "Rect.h"
#include "Types.h"
#include "Widget.h"

namespace GUI {

class Painter {
public:
    explicit Painter(Bitmap& bitmap);
    explicit Painter(Widget& widget);

    void setClipRect(const Rect& clipRect);
    void drawFilledRect(const Rect& rect, GUI::Color color);
    void drawRect(const Rect& rect, GUI::Color color);
    void drawFilledQuad(int x, int y, int size, GUI::Color color);
    void drawLine(const IntPoint& x, const IntPoint& y, GUI::Color color);
    void drawLine(int x0, int y0, int x1, int y1, GUI::Color color);
    void drawText(const Rect& rect, const ADS::String& text, Alignment alignment, GUI::Color color);
    void drawCharacterBitmap(const IntPoint& point, const CharacterBitmap& bitmap, Color color);
    void blit(IntPoint point, Bitmap& source);

private:
    Bitmap* m_targetBuffer { nullptr };
    int m_relativeTranslationX {};
    int m_relativeTranslationY {};
    Rect m_clipRect;
};

} // GUI
