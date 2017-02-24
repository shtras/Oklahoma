#include "StdAfx.h"
#include "TextInput.h"

namespace OGUI {
    TextInput::TextInput(Rect pos):
        Widget(pos),
        cursorX_(0),
        cursorY_(0),
        selectStartX_(0),
        selectEndX_(0),
        selectStartY_(0),
        selectEndY_(0),
        selectionUVs_({20, 177, 15, 9}),
        charWidth_(10),
        charHeight_(20),
        multiline_(false)
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
        text_.push_back(L"");
    }

    TextInput::~TextInput()
    {

    }

    void TextInput::Render()
    {
        Renderer& renderer = Renderer::GetInstance();
        Widget::Render();
        renderer.SetCharSize(charWidth_, charHeight_);
        for (int i = 0; i < text_.size(); ++i) {
            renderer.RenderText(text_[i].c_str(), pos_.left, pos_.top + i * charHeight_ * renderer.GetPixelHeight());
        }
        if (keyFocus_) {
            Rect cursorPos = { 0.0f, 0.0f, 10 * renderer.GetPixelWidth(), charHeight_*renderer.GetPixelHeight() };
            GetPosition(cursorX_, cursorY_, &cursorPos.left, &cursorPos.top);
            renderer.SetTexture(Renderer::TEX_FONT);
            renderer.RenderRect(cursorPos, cursorUVs_);
            if (HasSelection()) {
                int x1 = selectStartX_;
                int x2 = selectEndX_;
                int y1 = selectStartY_;
                int y2 = selectEndY_;
                if (y1 > y2) {
                    swap(y1, y2);
                    swap(x1, x2);
                }
                if (y1 == y2 && x2 < x1) {
                    swap(x1, x2);
                }
                for (int y = y1; y <= y2; ++y) {
                    Rect selectionPos = { pos_.left, pos_.top + y * charHeight_ * renderer.GetPixelHeight(), charWidth_ * text_[y].length() * renderer.GetPixelWidth(), charHeight_ * renderer.GetPixelHeight() };
                    if (y == y1) {
                        selectionPos.left += x1 * charWidth_ * renderer.GetPixelWidth();
                        selectionPos.width -= x1 * charWidth_ * renderer.GetPixelWidth();
                    }
                    if (y == y2) {
                        selectionPos.width -= (text_[y].length() - x2) * charWidth_ * renderer.GetPixelWidth();
                    }
                    renderer.RenderRect(selectionPos, selectionUVs_);
                }
            }
        }
        renderer.ResetCharSize();
    }

    void TextInput::HandleKeyboardEvent(SDL_Event& event)
    {
        wchar_t c = event.key.keysym.sym;
        if (event.key.type == SDL_KEYDOWN) {
            bool shift = IsShift(event.key.keysym);
            bool ctrl = IsCtrl(event.key.keysym);
            switch (event.key.keysym.sym)
            {
            case SDLK_BACKSPACE:
                if (!HasSelection()) {
                    selectStartX_ = cursorX_;
                    selectStartY_ = cursorY_;
                    MoveCursor(cursorX_ - 1, cursorY_, true);
                }
                Erase();
                break;
            case SDLK_DELETE:
                if (!HasSelection()) {
                    selectStartX_ = cursorX_;
                    selectStartY_ = cursorY_;
                    MoveCursor(cursorX_ + 1, cursorY_, true);
                }
                Erase();
                break;
            case SDLK_LEFT:
                if (ctrl) {
                    MoveCursor(FindWordBoundary(-1), cursorY_, shift);
                } else {
                    MoveCursor(cursorX_ - 1, cursorY_, shift);
                }
                break;
            case SDLK_RIGHT:
                if (ctrl) {
                    MoveCursor(FindWordBoundary(1), cursorY_, shift);
                } else {
                    MoveCursor(cursorX_ + 1, cursorY_, shift);
                }
                break;
            case SDLK_UP:
                if (multiline_) {
                    MoveCursor(cursorX_, cursorY_ - 1, shift);
                }
                break;
            case SDLK_DOWN:
                if (multiline_) {
                    MoveCursor(cursorX_, cursorY_ + 1, shift);
                }
                break;
            case SDLK_HOME:
                MoveCursor(0, cursorY_, shift);
                break;
            case SDLK_END:
                MoveCursor((int)text_[cursorY_].length(), cursorY_, shift);
                break;
            case SDLK_RETURN:
                if (multiline_) {
                    text_.insert(text_.begin() + cursorY_ + 1, text_[cursorY_].substr(cursorX_));
                    text_[cursorY_].erase(cursorX_);
                    MoveCursor(0, cursorY_ + 1, false);
                }
                break;
            default:
                if (event.key.keysym.sym & SDLK_SCANCODE_MASK) {
                    if (c >= SDL_SCANCODE_KP_1 && c <= SDL_SCANCODE_KP_9) {
                        c = c - SDL_SCANCODE_KP_1 + L'1';
                    } else if (c == SDL_SCANCODE_KP_0) {
                        c = L'0';
                    } else {
                        break;
                    }
                }
                if (Renderer::GetInstance().IsFontSymbol(c)) {
                    Erase();
                    c = GetChar(c, event.key.keysym);
                    text_[cursorY_].insert(cursorX_, 1, c);
                    MoveCursor(cursorX_ + 1, cursorY_, false);
                }
                break;
            }
        }
    }

    void TextInput::SetMultiline(bool val)
    {
        multiline_ = val;
        if (!val && text_.size() > 1) {
            assert(0);
        }
    }

    void TextInput::Erase()
    {
        if (!HasSelection()) {
            return;
        }
        int startY = selectStartY_;
        int endY = selectEndY_;
        int startX = selectStartX_;
        int endX = selectEndX_;
        if (endY < startY) {
            swap(startY, endY);
            swap(startX, endX);
        }

        if (selectStartX_ > selectEndX_) {
            swap(selectStartX_, selectEndX_);
        }

        if (startY != endY) {
            text_[startY].erase(startX);
            text_[endY].erase(0, endX);
            text_[startY] += text_[endY];
            text_.erase(text_.begin() + startY + 1, text_.begin() + endY + 1);
        } else {
            text_[selectStartY_].erase(selectStartX_, selectEndX_ - selectStartX_);
        }

        selectEndX_ = selectStartX_;
        selectEndY_ = selectStartY_;
        MoveCursor(selectStartX_, selectStartY_, false);
    }

    void TextInput::MoveCursor(int x, int y, bool shift)
    {
        if (!multiline_) {
            y = 0;
        }
        if (x < 0 && y > 0) {
            x = (int)text_[cursorY_ - 1].length();
            --y;
        } else if (x > 0 && cursorY_ < text_.size() && x > text_[cursorY_].length() && y < text_.size() - 1) {
            x = 0;
            ++y;
        }
        OHelpers::Clamp(y, 0, (int)text_.size() - 1);
        OHelpers::Clamp(x, 0, (int)text_[y].length());

        if (shift) {
            selectEndX_ = x;
            selectEndY_ = y;
        } else {
            selectStartX_ = selectEndX_ = x;
            selectStartY_ = selectEndY_ = y;
        }
        cursorX_ = x;
        cursorY_ = y;
    }

    void TextInput::HandleMouseEventSelf(SDL_Event& event, float x, float y)
    {
        switch (event.type)
        {
        case SDL_MOUSEMOTION:
            if (pressed_) {
                int cx, cy;
                GetCharPos(x, y, cx, cy);
                MoveCursor(cx, cy, true);
            }
            break;
        default:
            Widget::HandleMouseEventSelf(event, x, y);
            break;
        }
    }

    void TextInput::HandleMouseDown(float x, float y)
    {
        int cx, cy;
        GetCharPos(x, y, cx, cy);
        MoveCursor(cx, cy, false);
        Widget::HandleMouseDown(x, y);
    }

    void TextInput::GetCharPos(float fx, float fy, int& x, int& y)
    {
        x = (int)((fx - pos_.left) / (charWidth_ * Renderer::GetInstance().GetPixelWidth()));
        y = (int)((fy - pos_.top) / (charHeight_ * Renderer::GetInstance().GetPixelHeight()));
    }

    int TextInput::FindWordBoundary(int dx)
    {
        int res;
        bool inSpaces = false;
        for (res = cursorX_; ;) {
            res += dx;
            if (res <= 0 || res >= text_[cursorY_].length()) {
                break;
            }
            if (!IsWordChar(text_[cursorY_][res])) {
                break;
            }
            if (text_[cursorY_][res] == L' ') {
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

    bool TextInput::IsShift(SDL_Keysym& sym)
    {
        return (sym.mod & KMOD_LSHIFT) || (sym.mod & KMOD_RSHIFT);
    }

    bool TextInput::IsCtrl(SDL_Keysym& sym)
    {
        return (sym.mod & KMOD_LCTRL) || (sym.mod & KMOD_RCTRL);
    }

    bool TextInput::IsWordChar(wchar_t c)
    {
        return wordChars_.count(c) > 0;
    }

    void TextInput::GetPosition(int x, int y, float* fx, float* fy)
    {
        Renderer& renderer = Renderer::GetInstance();
        *fx = pos_.left + x * charWidth_ * renderer.GetPixelWidth();
        *fy = pos_.top + y * charHeight_ * renderer.GetPixelHeight();
    }

    bool TextInput::HasSelection()
    {
        return selectStartX_ != selectEndX_ || selectStartY_ != selectEndY_;
    }

    wchar_t TextInput::GetChar(wchar_t c, SDL_Keysym& sym)
    {
        wchar_t res = c;
        if ((sym.mod & KMOD_LSHIFT) || (sym.mod & KMOD_RSHIFT)) {
            if (shiftChars_.count(res) > 0) {
                res = shiftChars_[res];
            }
        }
        return res;
    }

}
