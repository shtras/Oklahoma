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
        void SetMultiline(bool val);
    protected:
        void Erase();
        void MoveCursor(int x, int y, bool shift);
        void HandleMouseEventSelf(SDL_Event& event, float x, float y) override;
        void HandleMouseDown(float x, float y) override;
        void GetCharPos(float fx, float fy, int& x, int& y);
        int FindWordBoundary(int dx);
        bool IsShift(SDL_Keysym& event);
        bool IsCtrl(SDL_Keysym& event);
        bool IsWordChar(wchar_t c);
        void GetPosition(int x, int y, float* fx, float* fy);
        bool HasSelection();
        void RenderCursor();
        std::vector<std::wstring> text_;
        wchar_t GetChar(wchar_t c, SDL_Keysym& sym);
        std::map<wchar_t, wchar_t> shiftChars_;
        int cursorX_;
        int cursorY_;
        int selectStartX_;
        int selectEndX_;
        int selectStartY_;
        int selectEndY_;
        Rect cursorUVs_;
        Rect selectionUVs_;
        std::set<wchar_t> wordChars_;
        int charWidth_;
        int charHeight_;
        bool multiline_;
        bool cursorBlink_;
    };
}
