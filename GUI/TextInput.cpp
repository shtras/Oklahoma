#include "StdAfx.h"
#include "TextInput.h"

namespace OGUI {
    TextInput::TextInput(Rect pos):
        Widget(pos),
        cursorX_(0),
        selectStart_(0),
        selectEnd_(0),
        selectionUVs_({20, 177, 15, 9})
    {
        Init({ 168, 172, 184, 187, 2, 4, 16, 19 });
        clickable_ = true;
        keyboardListener_ = true;
        for (wchar_t c = L'a'; c <= L'z'; ++c) {
            shiftChars_[c] = c - L'a' + L'A';
        }
        wstring str1 = L"1234567890-=[];'\\,./";
        wstring str2 = L"!@#$%^&*()_+{}:\"|<>?";
        assert(str1.length() == str2.length());
        for (int i = 0; i < str1.length(); ++i) {
            shiftChars_[str1[i]] = str2[i];
        }
        cursorUVs_ = { 5, 176, 12, 22 };
        Renderer::GetInstance().InitUVs(cursorUVs_, Renderer::TEX_FONT);
        Renderer::GetInstance().InitUVs(selectionUVs_, Renderer::TEX_FONT);
        wstring strWordChars = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";
        for (int i = 0; i < strWordChars.length(); ++i) {
            wordChars_.insert(strWordChars[i]);
        }
    }

    TextInput::~TextInput()
    {

    }

    void TextInput::Render()
    {
        Renderer& renderer = Renderer::GetInstance();
        Widget::Render();
        renderer.RenderText(text_.c_str(), pos_.left, pos_.top);
        if (keyFocus_) {
            Rect cursorPos = { pos_.left + (20 * cursorX_ - 5) / (float)renderer.GetWidth(), pos_.top, 0.01f, pos_.height };
            renderer.SetTexture(Renderer::TEX_FONT);
            renderer.RenderRect(cursorPos, cursorUVs_);
            if (selectStart_ != selectEnd_) {
                int x1 = selectStart_;
                int x2 = selectEnd_;
                if (x2 < x1) {
                    swap(x1, x2);
                }
                Rect selectionPos = { pos_.left + 20 * x1 / (float)renderer.GetWidth(), pos_.top, 20 * (x2 - x1) / (float)renderer.GetWidth(), pos_.height };
                renderer.RenderRect(selectionPos, selectionUVs_);
            }
        }
    }

    void TextInput::HandleKeyboardEvent(SDL_Event& event)
    {
        if (event.key.type == SDL_KEYDOWN) {
            bool shift = IsShift(event);
            bool ctrl = IsCtrl(event);
            switch (event.key.keysym.sym)
            {
            case SDLK_BACKSPACE:
                if (selectStart_ == selectEnd_) {
                    selectStart_ = cursorX_;
                    selectEnd_ = cursorX_ - 1;
                }
                Erase();
                break;
            case SDLK_DELETE:
                if (selectStart_ == selectEnd_) {
                    selectStart_ = cursorX_;
                    selectEnd_ = cursorX_ + 1;
                }
                Erase();
                break;
            case SDLK_LEFT:
                if (ctrl) {
                    MoveCursor(FindWordBoundary(-1), shift);
                } else {
                    MoveCursor(cursorX_ - 1, shift);
                }
                break;
            case SDLK_RIGHT:
                if (ctrl) {
                    MoveCursor(FindWordBoundary(1), shift);
                } else {
                    MoveCursor(cursorX_ + 1, shift);
                }
                break;
            case SDLK_HOME:
                MoveCursor(0, shift);
                break;
            case SDLK_END:
                MoveCursor((int)text_.length(), shift);
                break;
            default:
                if (Renderer::GetInstance().IsFontSymbol(event.key.keysym.sym)) {
                    Erase();
                    wchar_t c = GetChar(event.key.keysym);
                    text_.insert(cursorX_, 1, c);
                    ++cursorX_;
                }
                break;
            }
        }
    }

    void TextInput::Erase()
    {
        if (selectStart_ == selectEnd_) {
            return;
        }
        int startX = selectStart_;
        int endX = selectEnd_;
        OHelpers::Clamp(startX, 0, (int)text_.length());
        OHelpers::Clamp(endX, 0, (int)text_.length());
        if (startX > endX) {
            swap(startX, endX);
        }
        text_.erase(startX, endX - startX);
        selectEnd_ = selectStart_;
        MoveCursor(startX, false);
    }

    void TextInput::MoveCursor(int x, bool shift)
    {
        OHelpers::Clamp(x, 0, (int)text_.length());
        cursorX_ = x;
        if (shift) {
            selectEnd_ = cursorX_;
        } else {
            selectStart_ = selectEnd_ = cursorX_;
        }
    }

    void TextInput::HandleMouseEventSelf(SDL_Event& event, float x, float y)
    {
        switch (event.type)
        {
        case SDL_MOUSEMOTION:
            if (pressed_) {
                MoveCursor(GetCharPos(x, y), true);
            }
            break;
        default:
            Widget::HandleMouseEventSelf(event, x, y);
            break;
        }
    }

    void TextInput::HandleMouseDown(float x, float y)
    {
        MoveCursor(GetCharPos(x, y), false);
        Widget::HandleMouseDown(x, y);
    }

    int TextInput::GetCharPos(float x, float y)
    {
        return (int)((x - pos_.left) / (20 * Renderer::GetInstance().GetPixelWidth()));
    }

    int TextInput::FindWordBoundary(int dx)
    {
        int res;
        bool inSpaces = false;
        for (res = cursorX_; ;) {
            res += dx;
            if (res <= 0 || res >= text_.length()) {
                break;
            }
            if (!IsWordChar(text_[res])) {
                break;
            }
            if (text_[res] == L' ') {
                if (!inSpaces) {
                    inSpaces = true;
                }
            } else if (inSpaces) {
                res -= dx;
                break;
            }
        }
        return res;
    }

    bool TextInput::IsShift(SDL_Event& event)
    {
        return (event.key.keysym.mod & KMOD_LSHIFT) || (event.key.keysym.mod & KMOD_RSHIFT);
    }

    bool TextInput::IsCtrl(SDL_Event& event)
    {
        return (event.key.keysym.mod & KMOD_LCTRL) || (event.key.keysym.mod & KMOD_RCTRL);
    }

    bool TextInput::IsWordChar(wchar_t c)
    {
        return wordChars_.count(c) > 0;
    }

    wchar_t TextInput::GetChar(SDL_Keysym& sym)
    {
        wchar_t res = sym.sym;
        if ((sym.mod & KMOD_LSHIFT) || (sym.mod & KMOD_RSHIFT)) {
            if (shiftChars_.count(res) > 0) {
                res = shiftChars_[res];
            }
        }
        return res;
    }

}
