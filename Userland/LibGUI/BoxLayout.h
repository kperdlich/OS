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

private:
    Direction m_direction;
};

} // GUI
