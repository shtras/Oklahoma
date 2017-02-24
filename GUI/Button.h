#pragma once
#include "Widget.h"
#include "Label.h"

namespace OGUI
{
    class Button : public Widget
    {
    public:
        Button(Rect pos);
        ~Button();
        void SetText(wstring& str);
        void SetText(const wchar_t* str);
        void OnClick() override;
        void Bind();
        std::function<void(void)> F;
    protected:
        Label* label_;
    };
}
