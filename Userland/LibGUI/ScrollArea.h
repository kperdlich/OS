//
// Created by n3dry on 20.06.24.
//

#include "ScrollBar.h"
#include "Widget.h"

namespace GUI {

class ScrollArea : public Widget {
public:
    explicit ScrollArea(Widget* parent = nullptr);
    virtual const char* className() const override { return "ScrollView"; }
    virtual Size preferredSizeHint() const override;
    virtual Size minSizeHint() const override;

    void setWidget(Widget* widget);
    virtual bool acceptsFocus() const override { return false; }

protected:
    virtual void onPaintEvent(Event& event) override;
    virtual void onMouseMoveEvent(MouseEvent& event) override;
    virtual void onMouseDownEvent(MouseEvent& event) override;
    virtual void onMouseUpEvent(MouseEvent& event) override;
    virtual void onResizeEvent(ResizeEvent& event) override;

private:
    void updateWidgetSize();

private:
    Widget* m_widget { nullptr };
    ScrollBar* m_verticalScrollBar { nullptr };
    ScrollBar* m_horizontalScrollBar { nullptr };
};

} // GUI
