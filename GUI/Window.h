#pragma once
#include "Widget.h"

namespace OGUI
{
    class Window : public Widget
    {
    public:
        Window(Rect pos);
        ~Window();
        void Render() override;
    protected:
    };
}
