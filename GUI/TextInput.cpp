#include "StdAfx.h"
#include "TextInput.h"

namespace OGUI {
    TextInput::TextInput(Rect pos):
        Widget(pos),
        cursorX_(0)
    {
        Init({ 336, 339, 352, 354, 2, 4, 16, 18 });
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
        const OGraphics::Texture* tex = Renderer::GetInstance().GetTexture(Renderer::TEX_GUI);
        cursorUVs_ = {336 / (float)tex->GetWidth(), 20/(float)tex->GetHeight(), 7 / (float)tex->GetWidth(), 12 / (float)tex->GetHeight() };
    }

    TextInput::~TextInput()
    {

    }

    void TextInput::Render()
    {
        Widget::Render();
        Renderer::GetInstance().RenderText(text_.c_str(), pos_.left, pos_.top);
        if (keyFocus_) {
            Rect cursorPos = { pos_.left + 20 * cursorX_ / (float)Renderer::GetInstance().GetWidth(), pos_.top, 0.01f, pos_.height };
            Renderer::GetInstance().SetTexture(Renderer::TEX_GUI);
            Renderer::GetInstance().RenderRect(cursorPos, cursorUVs_);
        }
    }

    void TextInput::HandleKeyboardEvent(SDL_Event& event)
    {
        if (event.key.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym)
            {
            case SDLK_BACKSPACE:
                if (cursorX_ > 0) {
                    text_.erase(cursorX_-1, 1);
                    --cursorX_;
                }
                break;
            case SDLK_DELETE:
                text_.erase(cursorX_, 1);
                break;
            case SDLK_LEFT:
                if (cursorX_ > 0) {
                    --cursorX_;
                }
                break;
            case SDLK_RIGHT:
                if (cursorX_ < text_.length()) {
                    ++cursorX_;
                }
                break;
            default:
                if (Renderer::GetInstance().IsFontSymbol(event.key.keysym.sym)) {
                    wchar_t c = GetChar(event.key.keysym);
                    text_.insert(cursorX_, 1, c);
                    ++cursorX_;
                }
                break;
            }
        }
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
