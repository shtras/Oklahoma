#include "StdAfx.h"
#include "Button.h"

namespace OGUI
{

    Button::Button(Rect pos) :
        Widget(pos),
        label_(new Label({ 0.0f, 0.0f, 1.0f, 1.0f }))
    {
        AddWidget(label_);
        Init({ 168, 172, 184, 187, 2, 4, 16, 19 });
        //SetHoveredTexture(189, 2);
        SetPressedTexture(210, 2);
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
