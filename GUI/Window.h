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
        enum ScrollBarType { VERTICAL = 0, HORIZONTAL = 1, MAX_TYPE };
        ScrollBar(Window* wnd, Rect pos, ScrollBarType type);
        ~ScrollBar();
        void SetPosition(float y, float height);
        void Render() override;
        void HandleMouseEventSelf(SDL_Event& event, float x, float y) override;
    protected:
        void ScrollUp();
        void ScrollDown();
        std::shared_ptr<Button> arrow1_;
        std::shared_ptr<Button> arrow2_;
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
        void AddWidget(std::shared_ptr<Widget> widget) override;
        void Scroll(float dx, float dy);
        void OnChildMove(Widget* w) override;
        void HandleMouseEventSelf(SDL_Event& event, float x, float y) override;
    protected:
        void RecalcContents();
        std::shared_ptr<ScrollBar> scrollBars_[ScrollBar::MAX_TYPE];
        float totalHeight_;
        float scrollTop_;
        bool verticalScroll_;
        bool horizontalScroll_;
    };
}
