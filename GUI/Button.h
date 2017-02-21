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
        void SetText(wstring& str) override;
        void SetText(const wchar_t* str) override;
        void OnClick() override;
    protected:
        SmartPtr<Widget> label_;
    };
}
