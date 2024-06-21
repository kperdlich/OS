//
// Created by n3dry on 20.06.24.
//

#include "Types.h"
#include "Widget.h"

namespace GUI {

class ScrollBar : public Widget {
public:
    explicit ScrollBar(Widget* parent = nullptr);

    virtual const char* className() const override { return "ScrollBar"; }
    virtual Size preferredSizeHint() const override;
    virtual Size minSizeHint() const override;

    int singleStep() const { return m_singleStep; }
    int pageStep() const { return m_pageStep; }
    int value() const { return m_sliderValue; }

    void setSingleStep(int value) { m_singleStep = value; }
    void setPageStep(int value) { m_pageStep = value; }
    void setRange(int min, int max)
    {
        ASSERT(max > min);
        m_min = min;
        m_max = max;
    }

    ADS::Function<void(int)> onValueChanged { nullptr };

protected:
    virtual void onPaintEvent(Event& event) override;
    virtual void onMouseMoveEvent(MouseEvent& event) override;
    virtual void onMouseDownEvent(MouseEvent& event) override;
    virtual void onMouseUpEvent(MouseEvent& event) override;

private:
    int sliderLength() const;
    int calculateDraggingDelta(const IntPoint& newPosition) const;
    Rect scrollUpButtonRect() const;
    Rect scrollDownButtonRect() const;
    Rect sliderRect() const;

private:
    int m_min { 0 };
    int m_max { 99 };
    int m_singleStep { 1 };
    int m_pageStep { 10 };
    int m_sliderValue { 0 };
    IntPoint m_draggingStartPosition;
    int m_draggingStartValue { 0 };
    bool m_isDragging { false };
};

} // GUI
