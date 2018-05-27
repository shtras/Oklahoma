#pragma once
#include "Widget.h"

namespace OGUI
{
    class Label : public Widget
    {
    public:
        Label(Rect pos);
        ~Label();
        void SetText(std::wstring& str);
        void SetText(const wchar_t* str);
        void Render() override;
    protected:
        std::wstring text_;
    };
}
