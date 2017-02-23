#include "StdAfx.h"
#include "Window.h"

namespace OGUI
{

    Window::Window(Rect pos) :
        Widget(pos)
    {
        draggable_ = true;
        Init({2, 36, 328, 333, 2, 36, 225, 240});
        Renderer& renderer = Renderer::GetInstance();
        float scrollWidth = 50 / (float)renderer.GetWidth();
        float scrollHeight = 50 / (float)renderer.GetHeight();
        scrollBars_[ScrollBar::VERTICAL] = new ScrollBar({1.0f - scrollWidth, 0.0f, scrollWidth, 1.0f - scrollHeight}, ScrollBar::VERTICAL);
        scrollBars_[ScrollBar::HORIZONTAL] = new ScrollBar({0.0f, 1.0f - scrollHeight, 1.0f - scrollWidth, scrollHeight}, ScrollBar::HORIZONTAL);
        AddWidget(scrollBars_[ScrollBar::VERTICAL]);
        AddWidget(scrollBars_[ScrollBar::HORIZONTAL]);
    }

    Window::~Window()
    {

    }

    void Window::Render()
    {
        Renderer::GetInstance().SetBound(pos_);
        Widget::Render();
        
        Renderer::GetInstance().ResetBound();
    }

    void Window::ToggleScrollBar(ScrollBar::ScrollBarType type, bool val)
    {
        scrollBars_[type]->SetVisible(val);
    }

    ScrollBar::ScrollBar(Rect pos, ScrollBarType type) :
        Widget(pos)
    {
        visible_ = false;
        Renderer& renderer = Renderer::GetInstance();
        Init({351, 351, 366, 366, 40, 40, 46, 46});
        switch (type)
        {
        case OGUI::ScrollBar::VERTICAL:
            arrow1_ = new Button({ 0.0f, 0.0f, 1.0f, 0.1f });
            arrow1_->Init({351, 351, 367, 367, 22, 22, 38, 38});
            AddWidget(arrow1_);
            arrow2_ = new Button({ 0.0f, 0.9f, 1.0f, 0.1f });
            arrow2_->Init({ 351, 351, 367, 367, 38, 38, 22, 22 });
            AddWidget(arrow2_);
            break;
        case OGUI::ScrollBar::HORIZONTAL:
            arrow1_ = new Button({ 0.0f, 0.0f, 0.1f, 1.0f });
            arrow1_->Init({ 385, 385, 369, 369, 22, 22, 38, 38 });
            AddWidget(arrow1_);
            arrow2_ = new Button({ 0.9f, 0.0f, 0.1f, 1.0f });
            arrow2_->Init({ 369, 369, 385, 385, 22, 22, 38, 38 });
            AddWidget(arrow2_);
            break;
        default:
            assert(0);
            break;
        }
    }

    ScrollBar::~ScrollBar()
    {

    }

}
