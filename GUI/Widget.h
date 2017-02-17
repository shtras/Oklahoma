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
    private:
        Widget();
        void Init();

        TexturePos texPos_;
        Rect pos_;
        WidgetRects uvs_;
        WidgetRects rects_;
    };
}
