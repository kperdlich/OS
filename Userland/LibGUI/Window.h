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
#include "FocusReason.h"

namespace GUI {

class Window : public CObject {
public:
    explicit Window(CObject* parent = nullptr);
    ~Window() override;

    virtual bool event(Event& event) override;
    virtual bool isWindowType() const override;

    void show();
    void hide();
    void close();

    inline bool isVisible() { return m_isVisible; }

    bool contains(IntPoint position);
    void moveBy(int x, int y);

    void setRect(const Rect& rect);
    [[nodiscard]] inline Rect rect() const { return m_rect; }

    inline void setTitle(const ADS::String& title) { m_title = title; }
    inline ADS::String title() const { return m_title; }

    void setCentralWidget(Widget& widget);
    inline Widget* centralWidget() { return m_centralWidget; };

    Widget* focusedWidget() const { return m_focusedWidget; }
    void setFocusedWidget(Widget* widget, FocusReason reason);

    bool isActive() const;

private:
    ADS::String m_title {};
    Rect m_rect;
    Widget* m_centralWidget { nullptr };
    Widget* m_focusedWidget { nullptr };
    bool m_isVisible = true;
};

} // GUI
