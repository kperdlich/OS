//
// Created by n3dry on 15.06.24.
//

#pragma once

#include "Layout.h"

namespace GUI {

class BoxLayout : public Layout {
public:
    enum class Direction {
        Vertical,
        Horizontal
    };

    BoxLayout(Direction direction, Widget* parent = nullptr);
    virtual void activate() override;
    virtual Size preferredSizeHint() const override;
    virtual Size minSizeHint() const override;

private:
    Direction m_direction;
};

class HBoxLayout : public BoxLayout {
public:
    explicit HBoxLayout(Widget* parent = nullptr)
        : BoxLayout(Direction::Horizontal, parent)
    {
    }
};

class VBoxLayout : public BoxLayout {
public:
    explicit VBoxLayout(Widget* parent = nullptr)
        : BoxLayout(Direction::Vertical, parent)
    {
    }
};

} // GUI
