//
// Created by n3dry on 30.05.24.
//

#pragma once

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
    };

    explicit Event(Type type)
        : m_type(type)
    {
    }

    [[nodiscard]] Type type() const { return m_type; }

private:
    Type m_type { Type::Invalid };
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
