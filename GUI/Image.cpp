#include "StdAfx.h"
#include "Image.h"

namespace OGUI
{

    Image::Image(Rect pos, MainWindow* mw, Rect uvs, Renderer::TextureType type):
        Widget(pos, mw),
        uvs_(uvs)
    {
        Renderer::GetInstance().InitUVs(uvs, type);
    }

    Image::~Image()
    {

    }

    void Image::Render()
    {
        Renderer::GetInstance().RenderRect(screenPos_, uvs_);
    }

}
