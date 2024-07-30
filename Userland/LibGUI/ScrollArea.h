//
// Created by n3dry on 20.06.24.
//

#include "ScrollBar.h"
#include "Widget.h"

namespace GUI {

class ScrollArea : public Widget {
public:
    explicit ScrollArea(Widget* parent = nullptr);
    virtual const char* className() const override { return "ScrollArea"; }
    virtual Size preferredSizeHint() const override;
    virtual Size minSizeHint() const override;

    void setWidget(Widget* widget);
    virtual bool acceptsFocus() const override { return false; }

protected:
    virtual void onResizeEvent(ResizeEvent& event) override;
    void onPaintEvent(PaintEvent& event) override;

private:
    void updateScrollBars();
    void updateWidgetSize();
    Size availableContentSize() const;
    Rect visibleContentRect() const;

private:
    Widget* m_widget { nullptr };
    ADS::OwnPtr<ScrollBar> m_verticalScrollBar;
    ADS::OwnPtr<ScrollBar> m_horizontalScrollBar;
};

} // GUI
