//
// Created by n3dry on 17.09.22.
//

#pragma once

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

    virtual void onMouseMove(int x, int y);
    virtual void onMouseDown(int key, int x, int y);
    virtual void onMouseUp(int key, int x, int y);

    void render(Painter& painter);

private:
    bool m_isVisible = true;
    IntRect m_rect;
    bool m_isDragging = false;
    IntSize m_lastMouseMovePos { 0, 0 };
};

} // GUI
