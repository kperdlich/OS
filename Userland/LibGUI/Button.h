//
// Created by n3dry on 23.09.22.
//

#pragma once

#include "Painter.h"
#include "Widget.h"
#include "functional"

namespace GUI {

class Button : public Widget {
public:
    Button(Widget* parent, std::function<void()> onClickCallback);

    void onMouseDown(int key, int x, int y) override;
    void onMouseUp(int key, int x, int y) override;

    void render(Painter& painter, IntRect rect, Color color);
    void onMouseMove(int x, int y) override;

private:
    std::function<void()> m_onClick;
};

} // GUI
