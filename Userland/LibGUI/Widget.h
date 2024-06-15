//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "CObject.h"
#include "Event.h"
#include "FocusReason.h"
#include "Rect.h"

namespace GUI {

class Window;
class Layout;

class Widget : public CObject {
public:
    explicit Widget(Widget* parent = nullptr);

    virtual const char* className() const { return "Widget"; }

    virtual bool event(Event& event) override;
    virtual bool isWidgetType() const override;

    void setWindow(Window* window);
    Window* window() const { return m_window; }

    void setWindowRelativeRect(const Rect& rect);
    Rect windowRelativeRect() { return m_windowRelativeRect; }

    Rect rect() const { return { 0, 0, m_windowRelativeRect.width(), m_windowRelativeRect.height() }; }

    struct HitResult {
        Widget* widget;
        int localX;
        int localY;
    };

    bool hits(int x, int y, HitResult& result);

    bool hasFocus() const;
    void setFocus(FocusReason reason);

    bool isVisible() const { return m_isVisible; }
    void setIsVisible(bool value) { m_isVisible = value; }

    Layout* layout() const { return m_layout; }
    void setLayout(Layout* layout);

    Widget* parentWidget() const;

protected:
    virtual void onShowEvent(Event& event);
    virtual void onHideEvent(Event& event);
    virtual void onPaintEvent(Event& event);
    virtual void onMouseMoveEvent(MouseEvent& event);
    virtual void onMouseDownEvent(MouseEvent& event);
    virtual void onMouseUpEvent(MouseEvent& event);
    virtual void onKeyDownEvent(KeyEvent& event);
    virtual void onKeyUpEvent(KeyEvent& event);
    virtual void onFocusInEvent(FocusEvent& event);
    virtual void onFocusOutEvent(FocusEvent& event);
    virtual void onResizeEvent(ResizeEvent& event);

private:
    void updateLayout();

protected:
    Window* m_window;
    Rect m_windowRelativeRect;
    Layout* m_layout;
    bool m_isVisible { true };
};

} // GUI
