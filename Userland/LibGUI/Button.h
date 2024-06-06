//
// Created by n3dry on 23.09.22.
//

#pragma once

#include "Event.h"
#include "Types.h"
#include "Widget.h"
#include "functional"

namespace GUI {

class Button : public Widget {
public:
    Button(std::function<void()> onClickCallback, Widget* parent = nullptr);
    std::function<void()> onClick;

    void setText(const ADS::String& text);

    virtual const char* name() const override { return "Button"; }

protected:
    virtual void onPaintEvent(Event& event) override;
    virtual void onMouseDownEvent(MouseEvent& event) override;
    virtual void onMouseUpEvent(MouseEvent& event) override;
    virtual void onMouseMoveEvent(MouseEvent& event) override;

private:
    ADS::String m_text;
};

} // GUI
