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
    explicit Button(Widget* parent = nullptr);
    explicit Button(const ADS::String& text, Widget* parent = nullptr);
    Button(std::function<void()> onClickCallback, Widget* parent = nullptr);

    std::function<void()> onClick { nullptr };

    void setText(const ADS::String& text);

    virtual const char* className() const override { return "Button"; }

    virtual bool acceptsFocus() const { return true; }
    virtual Size preferredSizeHint() const override;
    virtual Size minSizeHint() const override;

protected:
    virtual void onPaintEvent(Event& event) override;
    virtual void onMouseDownEvent(MouseEvent& event) override;
    virtual void onMouseUpEvent(MouseEvent& event) override;
    virtual void onMouseMoveEvent(MouseEvent& event) override;

    static int fontWidth();

private:
    ADS::String m_text;
    bool m_isCurrentlyPressed { false };
};

} // GUI
