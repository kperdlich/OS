//
// Created by n3dry on 30.05.24.
//

#pragma once

#include "Point.h"
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
        Paint,
        Quit,
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
    IntPoint position() const { return m_position; }
    MouseButton button() const { return m_button; }

private:
    IntPoint m_position;
    MouseButton m_button;
};

enum class Key {
    Unknown, // FIXME: there shouldn't be any invalid/unknown key
    Backspace,
    Enter,
    Left,
    Up,
    Right,
    Down
};

class KeyEvent : public Event {
public:
    KeyEvent(Type type, Key key, const ADS::String& text)
        : m_key(key)
        , m_text(text)
        , Event(type)
    {
    }

    [[nodiscard]] Key key() const { return m_key; }
    [[nodiscard]] ADS::String text() const { return m_text; }

private:
    Key m_key;
    ADS::String m_text;
};

}
