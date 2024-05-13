//
// Created by n3dry on 23.09.22.
//

#pragma once

#include "Widget.h"
#include "functional"

namespace GUI {

class Button : public Widget {
public:
    Button(std::function<void()> onClickCallback, Widget* parent = nullptr);

    void onMouseDown(int key, int x, int y) override;
    void onMouseUp(int key, int x, int y) override;

    void onPaint() override;
    void onMouseMove(int x, int y) override;

    std::function<void()> onClick;
};

} // GUI
