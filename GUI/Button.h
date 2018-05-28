#pragma once
#include "Widget.h"
#include "Label.h"

namespace OGUI
{
    class Button : public Widget
    {
    public:
        Button(Rect pos, MainWindow* mw);
        ~Button();
        void SetText(std::wstring& str);
        void SetText(const wchar_t* str);
        void onClick() override;
        void Bind();
        std::function<void(void)> F;
    protected:
        std::shared_ptr<Label> label_ = nullptr;
    };
}
