#pragma once
#include "Widget.h"

namespace OGUI
{
    class Image : public Widget
    {
    public:
        Image(Rect pos, Rect uvs, Renderer::TextureType type);
        ~Image();
        void Render() override;
    protected:
        Rect uvs_;
    };
}
