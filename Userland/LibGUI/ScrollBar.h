//
// Created by n3dry on 20.06.24.
//

#include "Widget.h"

namespace GUI {

class ScrollBar : public Widget {
public:
    explicit ScrollBar(Widget* parent = nullptr);

    virtual const char* className() const override { return "ScrollBar"; }
    virtual Size preferredSizeHint() const override;
    virtual Size minSizeHint() const override;

    int value() const { return m_value; }
    void setStepSize(int value) { m_stepSize = value; }
    void setRange(int min, int max)
    {
        m_min = min;
        m_max = max;
    }

    void doSingleStepUp();
    void doSingleStepDown();

protected:
    virtual void onPaintEvent(Event& event) override;
    virtual void onMouseMoveEvent(MouseEvent& event) override;
    virtual void onMouseDownEvent(MouseEvent& event) override;
    virtual void onMouseUpEvent(MouseEvent& event) override;

private:
    Rect scrollUpButtonRect() const;
    Rect scrollDownButtonRect() const;
    Rect sliderRect() const;

private:
    int m_min { 0 };
    int m_max { 100 };
    int m_stepSize { 20 };
    int m_value { 0 };
    bool m_isDragging { false };
    IntPoint m_draggingStart;
};

} // GUI
