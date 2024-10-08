//
// Created by n3dry on 30.05.24.
//

#pragma once

#include "FocusReason.h"
#include "Key.h"
#include "Point.h"
#include "Rect.h"
#include "Size.h"
#include "Types.h"

namespace GUI {

// Based on https://doc.qt.io/qt-6/qevent.html#Type-enum
class Event {
public:
    enum class Type {
        Invalid = 0,
        MouseDown,
        MouseUp,
        MouseMove,
        KeyDown,
        KeyUp,
        FocusIn,
        FocusOut,
        Resize,
        Layout,
        Paint,
        Show,
        Hide,
        Timer,
        Quit,
        UpdateRequest,
    };

    explicit Event(Type type)
        : m_type(type)
    {
    }

    [[nodiscard]] Type type() const { return m_type; }

    bool isMouseEvent() const { return m_type == Type::MouseDown || m_type == Type::MouseUp || m_type == Type::MouseMove; }
    bool isKeyboardEvent() const { return m_type == Type::KeyUp || m_type == Type::KeyDown; }
    bool isPaintEvent() const { return m_type == Type::Paint; }

private:
    Type m_type { Type::Invalid };
};

enum class MouseButton {
    NoButton,
    Left,
    Right,
    Middle,
};

class MouseEvent : public Event {
public:
    MouseEvent(Type type, int x, int y, MouseButton button = MouseButton::NoButton)
        : m_position(x, y)
        , m_button(button)
        , Event(type)
    {
    }

    [[nodiscard]] int x() const { return m_position.x(); }
    [[nodiscard]] int y() const { return m_position.y(); }
    Point position() const { return m_position; }
    MouseButton button() const { return m_button; }

private:
    Point m_position;
    MouseButton m_button;
};

class KeyEvent : public Event {
public:
    KeyEvent(Type type, Key key, KeyboardModifier modifier, const ADS::String& text)
        : m_key(key)
        , m_modifier(modifier)
        , m_text(text)
        , Event(type)
    {
    }

    [[nodiscard]] bool ctrl() const { return m_modifier & KeyboardModifier::CtrlModifier; }
    [[nodiscard]] bool alt() const { return m_modifier & KeyboardModifier::AltModifier; }
    [[nodiscard]] bool shift() const { return m_modifier & KeyboardModifier::ShiftModifier; }
    [[nodiscard]] bool meta() const { return m_modifier & KeyboardModifier::MetaModifier; }
    [[nodiscard]] KeyboardModifier modifier() const { return m_modifier; }
    [[nodiscard]] Key key() const { return m_key; }
    [[nodiscard]] const ADS::String& text() const { return m_text; }

private:
    Key m_key;
    KeyboardModifier m_modifier;
    ADS::String m_text;
};

class Widget;

class UpdateEvent : public Event {
public:
    explicit UpdateEvent(Widget& widget, const Rect& rect)
        : m_widget(&widget)
        , m_rect(rect)
        , Event(Event::Type::UpdateRequest)
    {
    }

    [[nodiscard]] Widget* widget() const { return m_widget; }
    [[nodiscard]] Rect rect() const { return m_rect; }

private:
    Widget* m_widget { nullptr };
    Rect m_rect;
};

class PaintEvent : public Event {
public:
    explicit PaintEvent(const Rect& paintRect)
        : m_rect(paintRect)
        , Event(Type::Paint)
    {
    }

    [[nodiscard]] Rect rect() const { return m_rect; }

private:
    Rect m_rect;
};

class TimerEvent : public Event {
public:
    TimerEvent()
        : Event(Type::Timer)
    {
    }
};

class FocusEvent : public Event {
public:
    FocusEvent(Type type, FocusReason focusReason)
        : m_focusReason(focusReason)
        , Event(type)
    {
    }

    [[nodiscard]] FocusReason reason() const { return m_focusReason; }

private:
    FocusReason m_focusReason;
};

class ResizeEvent : public Event {
public:
    ResizeEvent(const Size& size, const Size& oldSize)
        : m_size(size)
        , m_oldSize(oldSize)
        , Event(Type::Resize)
    {
    }

    [[nodiscard]] const Size& oldSize() const { return m_oldSize; }
    [[nodiscard]] const Size& size() const { return m_size; }

private:
    Size m_size;
    Size m_oldSize;
};

}
