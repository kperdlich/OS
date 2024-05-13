//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "Button.h"
#include "CObject.h"
#include "Painter.h"
#include "Rect.h"
#include "Size.h"

namespace GUI {

class Painter;

class Window : public CObject {
public:
    explicit Window(CObject* parent = nullptr);
    ~Window() override;

    void show();
    inline bool isVisible() { return m_isVisible; }

    virtual void onPaint();
    virtual void onMouseMove(int x, int y);
    virtual void onMouseDown(int key, int x, int y);
    virtual void onMouseUp(int key, int x, int y);

    bool contains(int x, int y);
    void moveBy(int x, int y);

    void setRect(const IntRect& rect);
    [[nodiscard]] inline IntRect rect() const { return m_rect; }

    void setCentralWidget(Widget& widget);
    inline Widget* centralWidget() { return m_centralWidget; };

private:
    IntRect m_rect;
    Widget* m_centralWidget { nullptr };
    bool m_isVisible = true;
};

} // GUI
