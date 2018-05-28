#pragma once
#include "Widget.h"

namespace OGUI
{
    class TextInput : public Widget
    {
    public:
        TextInput(Rect pos, MainWindow* mw);
        ~TextInput();
        void Render() override;
        void HandleKeyboardEvent(SDL_Event& event);
        void SetMultiline(bool val);
    protected:
        void erase();
        void moveCursor(int x, int y, bool shift);
        void handleMouseEventSelf(SDL_Event& event, float x, float y) override;
        void handleMouseDown(float x, float y) override;
        void getCharPos(float fx, float fy, int& x, int& y);
        int findWordBoundary(int dx);
        bool isShift(SDL_Keysym& event);
        bool isCtrl(SDL_Keysym& event);
        bool isWordChar(wchar_t c);
        void getPosition(int x, int y, float* fx, float* fy);
        bool hasSelection();
        void renderCursor();
        wchar_t getChar(wchar_t c, SDL_Keysym& sym);

        std::vector<std::wstring> text_;
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
