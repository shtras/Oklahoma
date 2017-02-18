#pragma once
#include "Renderer.h"
#include "GUIInterface.h"
using namespace OGraphics;

namespace OGUI
{
    class Widget
    {
    public:
        Widget(Rect pos, TexturePos tex);
        virtual ~Widget();
        virtual void Render();
        void AddWidget(SmartPtr<Widget> widget);
    protected:
        Widget();
        void Init();
        void CreateUVs();
        void CreateRects();
        void Resize(Rect containingRect);
        void RenderChildren();
        Rect pos_;
        TexturePos texPos_;
        WidgetRects uvs_;
        WidgetRects rects_;
        list<SmartPtr<Widget>> children_;
    };
}
