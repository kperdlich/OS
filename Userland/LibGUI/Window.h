//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "Button.h"
#include "Painter.h"
#include "Rect.h"
#include "Size.h"

namespace GUI {

class Window final {
public:
    explicit Window(IntRect rect);
    ~Window() = default;

    inline void show() { m_isVisible = true; }
    inline void hide() { m_isVisible = false; }
    inline bool isVisible() { return m_isVisible; }

    virtual void onMouseMove(int x, int y);
    virtual void onMouseDown(int key, int x, int y);
    virtual void onMouseUp(int key, int x, int y);

    void render(Painter& painter);

    bool hits(int x, int y);

private:
    bool m_isVisible = true;
    bool m_isDragging = false;
    Button* m_closeButton;
    Button* m_hideButton;
    IntRect m_rect;
    IntSize m_lastMouseMovePos { 0, 0 };
};

} // GUI
