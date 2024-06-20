//
// Created by n3dry on 15.06.24.
//

#pragma once

#include "Widget.h"

namespace GUI {

struct LayoutItem {
    Widget* widget;
};

class Layout : public CObject {
public:
    explicit Layout(Widget* parent = nullptr);

    virtual void activate() = 0;
    void addWidget(Widget& widget);

    int spacing() const { return m_spacing; };
    void setSpacing(int value);

    virtual Size preferredSizeHint() const = 0;
    virtual Size minSizeHint() const = 0;

protected:
    ADS::Vector<LayoutItem> m_layoutItems;
    int m_spacing { 0 };
};

} // GUI
