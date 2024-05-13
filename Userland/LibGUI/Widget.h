//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "CObject.h"
#include "Rect.h"

namespace GUI {

class Painter;

class Widget : public CObject {
public:
    explicit Widget(Widget* parent = nullptr);

    virtual void onPaint();
    virtual void onMouseMove(int x, int y) = 0;
    virtual void onMouseDown(int key, int x, int y) = 0;
    virtual void onMouseUp(int key, int x, int y) = 0;

    void setRect(const IntRect& rect);
    inline IntRect rect() { return m_rect; }

protected:
    IntRect m_rect;
};

} // GUI
