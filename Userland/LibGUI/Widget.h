//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "CObject.h"
#include "Rect.h"

namespace GUI {

class Window;

class Widget : public CObject {
public:
    explicit Widget(Widget* parent = nullptr);

    virtual void onPaint();
    virtual void onMouseMove(int x, int y);
    virtual void onMouseDown(int key, int x, int y);
    virtual void onMouseUp(int key, int x, int y);
    virtual void focusInEvent();
    virtual void focusOutEvent();

    void setWindow(Window* window);
    Window* window() const { return m_window; }

    void setRect(const IntRect& rect);
    inline IntRect rect() { return m_rect; }

    bool hasFocus() const { return m_hasFocus; }
    void setFocus() { m_hasFocus = true; }

protected:
    Window* m_window;
    IntRect m_rect;
    bool m_hasFocus { false };
};

} // GUI
