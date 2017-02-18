#include "StdAfx.h"
#include "Widget.h"

namespace OGUI
{

    Widget::Widget(Rect pos, TexturePos tex):
        pos_(pos), texPos_(tex)
    {
        Init();
    }

    Widget::~Widget()
    {
    }

    void Widget::Init()
    {
        CreateUVs();
        CreateRects();
    }

    void Widget::CreateUVs()
    {
        Renderer& renderer = Renderer::GetInstance();
        int width = renderer.GetWidth();
        int height = renderer.GetHeight();
        const Texture* tex = renderer.GetTexture(Renderer::TEX_GUI);
        float fl1 = texPos_.l1 / (float)tex->GetWidth();
        float fl2 = texPos_.l2 / (float)tex->GetWidth();
        float fl3 = texPos_.l3 / (float)tex->GetWidth();
        float fl4 = texPos_.l4 / (float)tex->GetWidth();

        float ft1 = texPos_.t1 / (float)tex->GetHeight();
        float ft2 = texPos_.t2 / (float)tex->GetHeight();
        float ft3 = texPos_.t3 / (float)tex->GetHeight();
        float ft4 = texPos_.t4 / (float)tex->GetHeight();
        uvs_.topLeft = { fl1, ft1, fl2 - fl1, ft2 - ft1 };
        uvs_.top = { fl2, ft1, fl3 - fl2, ft2 - ft1 };
        uvs_.topRight = { fl3, ft1, fl4 - fl3, ft2 - ft1 };

        uvs_.left = { fl1, ft2, fl2 - fl1, ft3 - ft2 };
        uvs_.center = { fl2, ft2, fl3 - fl2, ft3 - ft2 };
        uvs_.right = { fl3, ft2, fl4 - fl3, ft3 - ft2 };

        uvs_.bottomLeft = { fl1, ft3, fl2 - fl1, ft4 - ft3 };
        uvs_.bottom = { fl2, ft3, fl3 - fl2, ft4 - ft3 };
        uvs_.bottomRight = { fl3, ft3, fl4 - fl3, ft4 - ft3 };
    }

    void Widget::CreateRects()
    {
        Renderer& renderer = Renderer::GetInstance();
        int width = renderer.GetWidth();
        int height = renderer.GetHeight();
        int w1 = texPos_.l2 - texPos_.l1;
        int w2 = texPos_.l3 - texPos_.l2;
        int w3 = texPos_.l4 - texPos_.l3;
        int h1 = texPos_.t2 - texPos_.t1;
        int h2 = texPos_.t3 - texPos_.t2;
        int h3 = texPos_.t4 - texPos_.t3;
        float fw1 = w1 / (float)width;
        float fw3 = w3 / (float)width;
        float fw2 = pos_.width - fw1 - fw3;
        float fh1 = h1 / (float)height;
        float fh3 = h3 / (float)height;
        float fh2 = pos_.height - fh1 - fh3;
        rects_.topLeft = { pos_.left, pos_.top, fw1, fh1 };
        rects_.top = { pos_.left + fw1, pos_.top, fw2, fh1 };
        rects_.topRight = { pos_.left + fw1 + fw2, pos_.top, fw3, fh1 };
        rects_.left = { pos_.left, pos_.top + fh1, fw1, fh2 };
        rects_.center = { pos_.left + fw1, pos_.top + fh1, fw2, fh2 };
        rects_.right = { pos_.left + fw1 + fw2, pos_.top + fh1, fw3, fh2 };
        rects_.bottomLeft = { pos_.left, pos_.top + fh1 + fh2, fw1, fh3 };
        rects_.bottom = { pos_.left + fw1, pos_.top + fh1 + fh2, fw2, fh3 };
        rects_.bottomRight = { pos_.left + fw1 + fw2, pos_.top + fh1 + fh2, fw3, fh3 };
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
        for (auto& itr : children_) {
            itr->Render();
        }
    }

    void Widget::Resize(Rect containingRect)
    {
        pos_.left = containingRect.left + pos_.left * containingRect.width;
        pos_.top = containingRect.top + pos_.top * containingRect.width;
        pos_.width *= containingRect.width;
        pos_.height *= containingRect.height;
        CreateRects();
    }

    void Widget::AddWidget(SmartPtr<Widget> widget)
    {
        children_.push_back(widget);
        widget->Resize(pos_);
    }

}
