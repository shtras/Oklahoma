#include "StdAfx.h"
#include "Label.h"

namespace OGUI {

    Label::Label(Rect pos):
        Widget(pos)
    {
        interactive_ = false;
    }

    Label::~Label()
    {

    }

    void Label::SetText(wstring& str)
    {
        text_ = str;
    }

    void Label::SetText(const wchar_t* str)
    {
        text_ = str;
    }

    void Label::Render()
    {
        Renderer::GetInstance().RenderText(text_.c_str(), pos_.left, pos_.top);
    }

}
