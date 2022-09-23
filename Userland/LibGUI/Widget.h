//
// Created by n3dry on 17.09.22.
//

#pragma once

#include "Rect.h"

namespace GUI {

class Widget {
public:
    explicit Widget(Widget* parent);

    virtual void onMouseMove(int x, int y) = 0;
    virtual void onMouseDown(int key, int x, int y) = 0;
    virtual void onMouseUp(int key, int x, int y) = 0;

protected:
    class Widget* m_parent;
};

} // GUI
