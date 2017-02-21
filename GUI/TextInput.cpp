#include "StdAfx.h"
#include "TextInput.h"

namespace OGUI {
    TextInput::TextInput(Rect pos):
        Widget(pos)
    {
        Init({ 336, 339, 352, 354, 2, 4, 16, 18 });
        clickable_ = true;
        keyboardListener_ = true;
    }

    TextInput::~TextInput()
    {

    }

    void TextInput::Render()
    {
        Widget::Render();
        Renderer::GetInstance().RenderText(text_.c_str(), pos_.left, pos_.top);
    }

    void TextInput::HandleKeyboardEvent(SDL_Event& event)
    {
        if (event.key.type == SDL_KEYDOWN) {
            if (Renderer::GetInstance().IsFontSymbol(event.key.keysym.sym)) {
                text_ += event.key.keysym.sym;
            }
        }
    }

}
