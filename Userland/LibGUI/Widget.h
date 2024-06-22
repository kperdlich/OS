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

    [[nodiscard]] Rect windowRelativeRect() const;

    void resize(int width, int height);
    void resize(const Size& newSize);

    [[nodiscard]] IntPoint relativePosition() const { return m_relativeRect.position(); }
    [[nodiscard]] Rect relativeRect() const { return m_relativeRect; }
    void setRelativeRect(const Rect& rect);

    Rect rect() const { return { 0, 0, m_relativeRect.width(), m_relativeRect.height() }; }

    int width() const { return m_relativeRect.width(); };
    int height() const { return m_relativeRect.height(); };

    struct HitResult {
        Widget* widget;
        IntPoint localPosition;
    };

    enum class SizePolicy {
        Fixed,
        Automatic,
    };

    [[nodiscard]] bool hits(const IntPoint& point, HitResult& result);

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

    void setFixedSize(const Size& value);

    [[nodiscard]] Size size() const { return m_relativeRect.size(); }

    void setVerticalSizePolicy(SizePolicy value) { m_verticalSizePolicy = value; }
    void setHorizontalSizePolicy(SizePolicy value) { m_horizontalSizePolicy = value; }

    [[nodiscard]] SizePolicy verticalSizePolicy() const { return m_verticalSizePolicy; }
    SizePolicy horizontalSizePolicy() const { return m_horizontalSizePolicy; }

    [[nodiscard]] Size minimumSize() const { return m_minimumSize; }

    virtual Size preferredSizeHint() const;
    virtual Size minSizeHint() const;

    void updateLayout();

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

protected:
    Window* m_window { nullptr };
    Layout* m_layout { nullptr };
    Rect m_relativeRect;
    Size m_minimumSize {};
    SizePolicy m_verticalSizePolicy { SizePolicy::Automatic };
    SizePolicy m_horizontalSizePolicy { SizePolicy::Automatic };
    bool m_isVisible { true };
};

} // GUI
