#include "StdAfx.h"
#include "Widget.h"

namespace OGUI
{

    Widget::Widget(Rect pos, TexturePos tex):
        pos_(pos), texPos_(tex)
    {
        Init();
    }

    void Widget::Init()
    {
        Renderer::GetInstance().InitWidgetTex(pos_, texPos_, uvs_, rects_);

//         uvs_.topLeft = { 0.0f, 0.0f, 1.0f, 1.0f };
//         uvs_.top = { 0.0f, 0.0f, 1.0f, 1.0f };
//         uvs_.topRight = { 0.0f, 0.0f, 1.0f, 1.0f };
//         uvs_.left = { 0.0f, 0.0f, 1.0f, 1.0f };
//         uvs_.center = { 0.0f, 0.0f, 1.0f, 1.0f };
//         uvs_.right = { 0.0f, 0.0f, 1.0f, 1.0f };
//         uvs_.bottomLeft = { 0.0f, 0.0f, 1.0f, 1.0f };
//         uvs_.bottom = { 0.0f, 0.0f, 1.0f, 1.0f };
//         uvs_.bottomRight = { 0.0f, 0.0f, 1.0f, 1.0f };
    }

    Widget::~Widget()
    {

    }

    void Widget::Render()
    {
        Renderer& renderer = Renderer::GetInstance();
        renderer.SetTexture(Renderer::TEX_GUI);
        renderer.RenderRect(rects_.topLeft, uvs_.topLeft);
        renderer.RenderRect(rects_.top, uvs_.top);
        renderer.RenderRect(rects_.topRight, uvs_.topRight);
        renderer.RenderRect(rects_.left, uvs_.left);
        renderer.RenderRect(rects_.center, uvs_.center);
        renderer.RenderRect(rects_.right, uvs_.right);
        renderer.RenderRect(rects_.bottomLeft, uvs_.bottomLeft);
        renderer.RenderRect(rects_.bottom, uvs_.bottom);
        renderer.RenderRect(rects_.bottomRight, uvs_.bottomRight);
    }

}
