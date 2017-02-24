#pragma once
#include "Widget.h"
#include "Button.h"
#include "Image.h"

namespace OGUI
{
    class Window;

    class ScrollBar : public Widget
    {
    public:
        enum ScrollBarType { VERTICAL = 0, HORIZONTAL = 1, MAX_TYPE=HORIZONTAL };
        ScrollBar(Window* wnd, Rect pos, ScrollBarType type);
        ~ScrollBar();
        void SetPosition(float y, float height);
        void Render() override;
    protected:
        void ScrollUp();
        void ScrollDown();
        Button* arrow1_;
        Button* arrow2_;
        Window* wnd_;
        float y_;
        float height_;
        Rect positionUVs_;
    };

    class Window : public Widget
    {
    public:
        Window(Rect pos);
        ~Window();
        void Render() override;
        void ToggleScrollBar(ScrollBar::ScrollBarType type, bool val);
        void AddWidget(SmartPtr<Widget> widget) override;
        void Scroll(float dx, float dy);
        void OnChildMove(Widget* w) override;
    protected:
        void RecalcContents();
        ScrollBar* scrollBars_[ScrollBar::MAX_TYPE + 1];
        float totalHeight_;
        float scrollTop_;
    };
}
