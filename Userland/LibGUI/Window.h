//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "Button.h"
#include "CObject.h"
#include "Event.h"
#include "Point.h"
#include "Painter.h"
#include "Rect.h"
#include "Size.h"

namespace GUI {

class Painter;

class Window : public CObject {
public:
    explicit Window(CObject* parent = nullptr);
    ~Window() override;

    virtual bool event(Event& event) override;
    virtual bool isWindowType() const override;

    void show();
    inline bool isVisible() { return m_isVisible; }

    bool contains(IntPoint position);
    void moveBy(int x, int y);

    void setRect(const IntRect& rect);
    [[nodiscard]] inline IntRect rect() const { return m_rect; }

    inline void setTitle(const ADS::String& title) { m_title = title; }
    inline ADS::String title() const { return m_title; }

    void setCentralWidget(Widget& widget);
    inline Widget* centralWidget() { return m_centralWidget; };

    Widget* focusedWidget() const { return m_focusedWidget; }
    void setFocusedWidget(Widget* widget);

private:
    ADS::String m_title {};
    IntRect m_rect;
    Widget* m_centralWidget { nullptr };
    Widget* m_focusedWidget { nullptr };
    bool m_isVisible = true;
};

} // GUI
