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
        wchar_t GetChar(SDL_Keysym& sym);
        map<wchar_t, wchar_t> shiftChars_;
        size_t cursorX_;
        Rect cursorUVs_;
    };
}
