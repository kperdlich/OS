//
// Created by n3dry on 23.09.22.
//

#pragma once

#include "Types.h"
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

    void setText(const ADS::String& text);

    virtual const char* name() const override { return "Button"; }

private:
    ADS::String m_text;
};

} // GUI
