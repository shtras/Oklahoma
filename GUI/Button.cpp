#include "StdAfx.h"
#include "Button.h"

namespace OGUI
{

    Button::Button(Rect pos) :
        Widget(pos),
        label_(new Label({ 0.0f, 0.0f, 1.0f, 1.0f }))
    {
        AddWidget(label_);
        Init({ 336, 339, 352, 354, 2, 4, 16, 18 });
        SetHoveredTexture(357, 2);
        SetPressedTexture(378, 2);
        clickable_ = true;
    }

    Button::~Button()
    {
    }

    void Button::SetText(wstring& str)
    {
        label_->SetText(str);
    }

    void Button::SetText(const wchar_t* str)
    {
        label_->SetText(str);
    }

    void Button::OnClick()
    {
        if (F) {
            F();
        }
    }

    void Button::Bind()
    {

    }

}
