#pragma once
#include "Widget.h"

namespace OGUI
{
    class Label : public Widget
    {
    public:
        Label(Rect pos);
        ~Label();
        void SetText(wstring& str);
        void SetText(const wchar_t* str);
        void Render() override;
    protected:
        wstring text_;
    };
}
