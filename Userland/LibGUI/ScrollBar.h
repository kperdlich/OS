//
// Created by n3dry on 20.06.24.
//

#include "CharacterBitmap.h"
#include "Orientation.h"
#include "Types.h"
#include "Widget.h"

namespace GUI {

class ScrollBar : public Widget {
public:
    explicit ScrollBar(Orientation orientation, Widget* parent = nullptr);

    [[nodiscard]] virtual const char* className() const override { return "ScrollBar"; }
    [[nodiscard]] virtual Size preferredSizeHint() const override;
    [[nodiscard]] virtual Size minSizeHint() const override;

    [[nodiscard]] int singleStep() const { return m_singleStep; }
    [[nodiscard]] int pageStep() const { return m_pageStep; }
    [[nodiscard]] int value() const { return m_sliderValue; }
    [[nodiscard]] bool hasSlider() const;

    void setSingleStep(int value) { m_singleStep = value; }
    void setPageStep(int value) { m_pageStep = value; }
    void setRange(int min, int max);
    void setValue(int value);

    ADS::Function<void(int)> onValueChanged { nullptr };

protected:
    virtual void onPaintEvent(PaintEvent& event) override;
    virtual void onMouseMoveEvent(MouseEvent& event) override;
    virtual void onMouseDownEvent(MouseEvent& event) override;
    virtual void onMouseUpEvent(MouseEvent& event) override;

private:
    [[nodiscard]] int sliderLength() const;
    [[nodiscard]] int calculateDraggingDelta(const Point& newPosition) const;
    [[nodiscard]] Rect scrollUpButtonRect() const;
    [[nodiscard]] Rect scrollDownButtonRect() const;
    [[nodiscard]] Rect sliderRect() const;
    [[nodiscard]] const CharacterBitmap& scrollButtonUpBitmap() const;
    [[nodiscard]] const CharacterBitmap& scrollButtonDownBitmap() const;
    [[nodiscard]] int availableRange() const;
    [[nodiscard]] int scrollButtonSize() const { return m_orientation == Orientation::Vertical ? width() : height(); }

private:
    Orientation m_orientation;
    int m_min { 0 };
    int m_max { 99 };
    int m_singleStep { 1 };
    int m_pageStep { 10 };
    int m_sliderValue { 0 };
    Point m_draggingStartPosition;
    int m_draggingStartValue { 0 };
    bool m_isDragging { false };
};

} // GUI
