//
// Created by n3dry on 11.06.24.
//

#pragma once

#include "Alignment.h"
#include "Types.h"
#include "Widget.h"

namespace GUI {

class Label : public Widget {
public:
    explicit Label(Widget* parent = nullptr);
    Label(const ADS::String& text, Widget* parent = nullptr);

    void setText(const ADS::String& text) { m_text = text; }
    [[nodiscard]] ADS::String text() const { return m_text; }

    void setAlignment(Alignment alignment) { m_alignment = alignment; }
    [[nodiscard]] Alignment alignment() const { return m_alignment; }

    virtual const char* className() const override { return "Label"; }
    virtual Size preferredSizeHint() const override;
    virtual Size minSizeHint() const override;

    void shrinkToFit();
protected:
    virtual void onPaintEvent(PaintEvent& event) override;

private:
    static int fontWidth();

private:
    ADS::String m_text;
    Alignment m_alignment { Alignment::Left };
};

}
