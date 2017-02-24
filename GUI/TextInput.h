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
        void Erase();
        void MoveCursor(int x, bool shift);
        wstring text_;
        wchar_t GetChar(SDL_Keysym& sym);
        map<wchar_t, wchar_t> shiftChars_;
        int cursorX_;
        int selectStart_;
        int selectEnd_;
        Rect cursorUVs_;
        Rect selectionUVs_;
    };
}
