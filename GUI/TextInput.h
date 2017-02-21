#pragma once
#include "Widget.h"

namespace OGUI
{
    class TextInput : public Widget
    {
    public:
        TextInput(Rect pos);
        ~TextInput();
        void Render() override;
        void HandleKeyboardEvent(SDL_Event& event);
    protected:
        wstring text_;
    };
}
