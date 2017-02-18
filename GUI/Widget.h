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
        bool HandleMouseEvent(SDL_Event& event, float x, float y);
        void SetHovered(bool val);
        void Init(TexturePos texPos);
        void SetHoveredTexture(int hoveredX, int hoveredY);
    protected:
        Widget();
        void CreateUVs(WidgetRects& uvs, TexturePos& texPos);
        void CreateRects();
        void Resize(Rect containingRect);
        void RenderChildren();
        bool IsWithin(float x, float y);
        virtual void HandleMouseEventSelf(SDL_Event& event, float x, float y);

        Rect pos_;
        TexturePos texPos_;
        WidgetRects uvs_;
        WidgetRects hoveredUVs_;
        WidgetRects rects_;
        list<SmartPtr<Widget>> children_;
        int texState_;
        bool hovered_;
        bool visible_;
    };
}
