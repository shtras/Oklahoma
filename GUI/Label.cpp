#include "StdAfx.h"
#include "Label.h"

namespace OGUI {

    Label::Label(Rect pos, MainWindow* mw):
        Widget(pos, mw)
    {
        interactive_ = false;
    }

    Label::~Label()
    {

    }

    void Label::SetText(std::wstring& str)
    {
        text_ = str;
    }

    void Label::SetText(const wchar_t* str)
    {
        text_ = str;
    }

    void Label::Render()
    {
        if (text_.length() > 0) {
            Renderer::GetInstance().RenderText(text_.c_str(), screenPos_.left, screenPos_.top);
        }
    }

}
