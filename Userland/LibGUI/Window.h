//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "Bitmap.h"
#include "Button.h"
#include "CObject.h"
#include "Event.h"
#include "FocusReason.h"
#include "Painter.h"
#include "Point.h"
#include "Rect.h"
#include "Size.h"

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

    bool isVisible() { return m_isVisible; }
    Size size() const { return m_rect.size(); }

    bool contains(Point position);
    void moveBy(int x, int y);

    void resize(Size size);
    void resize(int width, int height);
    void setPosition(int x, int y);
    void setPosition(const Point& point);
    [[nodiscard]] Rect rect() const { return m_rect; }
    [[nodiscard]] Point position() const { return m_rect.position(); }

    inline void setTitle(const ADS::String& title) { m_title = title; }
    inline ADS::String title() const { return m_title; }

    void setCentralWidget(Widget& widget);
    inline Widget* centralWidget() { return m_centralWidget; };

    Widget* focusedWidget() const { return m_focusedWidget; }
    void setFocusedWidget(Widget* widget, FocusReason reason);

    bool isActive() const;

    Bitmap* backBuffer() { return m_backBuffer.get(); }

private:
    ADS::UniquePtr<Bitmap> m_backBuffer;
    ADS::String m_title {};
    Rect m_rect { 100, 100, 640, 480 };
    Widget* m_centralWidget { nullptr };
    Widget* m_focusedWidget { nullptr };
    bool m_isVisible = true;
};

} // GUI
