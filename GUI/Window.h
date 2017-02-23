#pragma once
#include "Widget.h"
#include "Button.h"

namespace OGUI
{
    class ScrollBar : public Widget
    {
    public:
        enum ScrollBarType { VERTICAL = 0, HORIZONTAL = 1, MAX_TYPE=HORIZONTAL };
        ScrollBar(Rect pos, ScrollBarType type);
        ~ScrollBar();
    protected:
        Button* arrow1_;
        Button* arrow2_;
    };

    class Window : public Widget
    {
    public:
        Window(Rect pos);
        ~Window();
        void Render() override;
        void ToggleScrollBar(ScrollBar::ScrollBarType type, bool val);
    protected:
        ScrollBar* scrollBars_[ScrollBar::MAX_TYPE + 1];
    };
}
