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
        void HandleMouseEventSelf(SDL_Event& event, float x, float y) override;
        void HandleMouseDown(float x, float y) override;
        int GetCharPos(float x, float y);
        int FindWordBoundary(int dx);
        bool IsShift(SDL_Keysym& event);
        bool IsCtrl(SDL_Keysym& event);
        bool IsWordChar(wchar_t c);
        wstring text_;
        wchar_t GetChar(wchar_t c, SDL_Keysym& sym);
        map<wchar_t, wchar_t> shiftChars_;
        int cursorX_;
        int selectStart_;
        int selectEnd_;
        Rect cursorUVs_;
        Rect selectionUVs_;
        set<wchar_t> wordChars_;
    };
}
