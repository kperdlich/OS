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
    [[nodiscard]] Window* window() const { return m_window; }

    void setWindowRelativeRect(const Rect& rect);
    Rect windowRelativeRect() { return m_windowRelativeRect; }

    Rect rect() const { return { 0, 0, m_windowRelativeRect.width(), m_windowRelativeRect.height() }; }

    struct HitResult {
        Widget* widget;
        int localX;
        int localY;
    };

    enum class SizePolicy {
        Fixed,
        Automatic,
    };

    [[nodiscard]] bool hits(int x, int y, HitResult& result);

    [[nodiscard]] bool hasFocus() const;
    void setFocus(FocusReason reason);

    void grabMouse();
    void releaseMouse();

    [[nodiscard]] bool isVisible() const { return m_isVisible; }
    void setIsVisible(bool value) { m_isVisible = value; }

    Layout* layout() const { return m_layout; }
    void setLayout(Layout* layout);

    [[nodiscard]] Widget* parentWidget() const;

    virtual bool acceptsFocus() const { return false; }

    void setFixedSize(Size value) { m_windowRelativeRect.setSize(value); }

    [[nodiscard]] Size size() const { return m_windowRelativeRect.size(); }

    void setVerticalSizePolicy(SizePolicy value) { m_verticalSizePolicy = value; }
    void setHorizontalSizePolicy(SizePolicy value) { m_horizontalSizePolicy = value; }

    [[nodiscard]] SizePolicy verticalSizePolicy() const { return m_verticalSizePolicy; }
    SizePolicy horizontalSizePolicy() const { return m_horizontalSizePolicy; }

    virtual Size preferredSizeHint() const;
    virtual Size minSizeHint() const;

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
    Window* m_window { nullptr };
    Layout* m_layout { nullptr };
    Rect m_windowRelativeRect;
    SizePolicy m_verticalSizePolicy { SizePolicy::Automatic };
    SizePolicy m_horizontalSizePolicy { SizePolicy::Automatic };
    bool m_isVisible { true };
};

} // GUI
