#pragma once
#include "Renderer.h"
#include "GUIInterface.h"
using namespace OGraphics;

namespace OGUI
{
    class Widget
    {
        enum TextureState
        {
            NONE=0,
            REGULAR=1,
            HOVERED=2
        };
    public:
        Widget(Rect pos);
        virtual ~Widget();
        virtual void Render();
        void AddWidget(SmartPtr<Widget> widget);
        virtual bool HandleMouseEvent(SDL_Event& event, float x, float y);
        void ToggleHovered(bool val);
        void TogglePressed(bool val);
        void ToggleDragged(bool val);
        void Init(TexturePos texPos);
        void SetHoveredTexture(int hoveredX, int hoveredY);
        void Move(float x, float y, float dx, float dy);
    protected:
        Widget();
        Widget(const Widget& other);
        void CreateUVs(WidgetRects& uvs, TexturePos& texPos);
        void CreateRects();
        void Resize(Rect containingRect);
        void RenderChildren();
        bool IsWithin(float x, float y);
        virtual void HandleMouseEventSelf(SDL_Event& event, float x, float y);
        virtual void HandleMouseDown(float x, float y);
        virtual void HandleMouseUp(float x, float y);
        void SetParent(Widget* w);
        void MoveToTop(Widget* w);
        void Move(float dx, float dy);
        Rect pos_;
        TexturePos texPos_;
        WidgetRects uvs_;
        WidgetRects hoveredUVs_;
        OGraphics::Rect rects_[9];
        list<SmartPtr<Widget>> children_;
        int texState_;
        bool hovered_;
        bool pressed_;
        bool dragged_;
        bool visible_;

        bool clickable_;
        bool draggable_;
        float dragStartX_;
        float dragStartY_;
        Widget* parent_;
    };
}
