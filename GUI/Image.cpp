#include "StdAfx.h"
#include "Image.h"

namespace OGUI
{

    Image::Image(Rect pos, Rect uvs, Renderer::TextureType type):
        Widget(pos),
        uvs_(uvs)
    {
        Renderer::GetInstance().InitUVs(uvs, type);
    }

    Image::~Image()
    {

    }

    void Image::Render()
    {
        Renderer::GetInstance().RenderRect(pos_, uvs_);
    }

}
