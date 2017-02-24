#include "StdAfx.h"
#include "Window.h"

namespace OGUI
{

    Window::Window(Rect pos) :
        Widget(pos),
        totalHeight_(1.0f),
        scrollTop_(0)
    {
        draggable_ = true;
        Init({2, 36, 328, 333, 2, 36, 225, 240});
        Renderer& renderer = Renderer::GetInstance();
        float scrollWidth = 50 / (float)renderer.GetWidth();
        float scrollHeight = 50 / (float)renderer.GetHeight();
        scrollBars_[ScrollBar::VERTICAL] = new ScrollBar(this, {1.0f - scrollWidth, 0.0f, scrollWidth, 1.0f - scrollHeight}, ScrollBar::VERTICAL);
        scrollBars_[ScrollBar::HORIZONTAL] = new ScrollBar(this, {0.0f, 1.0f - scrollHeight, 1.0f - scrollWidth, scrollHeight}, ScrollBar::HORIZONTAL);
        AddWidget(scrollBars_[ScrollBar::VERTICAL]);
        AddWidget(scrollBars_[ScrollBar::HORIZONTAL]);
    }

    Window::~Window()
    {

    }

    void Window::Render()
    {
        Renderer::GetInstance().PushBound(pos_);
        Widget::Render();
        Renderer::GetInstance().PopBound();
    }

    void Window::ToggleScrollBar(ScrollBar::ScrollBarType type, bool val)
    {
        scrollBars_[type]->SetVisible(val);
    }

    void Window::AddWidget(SmartPtr<Widget> widget)
    {
        Widget::AddWidget(widget);
        RecalcContents();
    }

    void Window::Scroll(float dx, float dy)
    {
        if (scrollTop_ + dy > totalHeight_ - 1.0f) {
            dy = totalHeight_ - 1.0f - scrollTop_;
        }
        if (scrollTop_ + dy < 0) {
            dy = -scrollTop_;
        }
        scrollTop_ += dy;
        for (auto& itr : children_) {
            if (itr == scrollBars_[ScrollBar::VERTICAL] || itr == scrollBars_[ScrollBar::HORIZONTAL]) {
                continue;
            }
            itr->Move(dx * pos_.width, -dy * pos_.height);
        }
        scrollBars_[ScrollBar::VERTICAL]->SetPosition(scrollTop_ / totalHeight_, 1.0f / totalHeight_);
    }

    void Window::RecalcContents()
    {
        totalHeight_ = 1.0f;
        for (auto& itr : children_) {
            if (itr == scrollBars_[ScrollBar::VERTICAL] || itr == scrollBars_[ScrollBar::HORIZONTAL]) {
                continue;
            }
            float y = (itr->pos_.top + itr->pos_.height - pos_.top + scrollTop_*pos_.height) / pos_.height;
            if (y > totalHeight_) {
                totalHeight_ = y;
            }
        }
        scrollBars_[ScrollBar::VERTICAL]->SetPosition(scrollTop_ / totalHeight_, 1.0f / totalHeight_);
    }

    void Window::OnChildMove(Widget* w)
    {
        RecalcContents();
    }

    ScrollBar::ScrollBar(Window* wnd, Rect pos, ScrollBarType type) :
        Widget(pos),
        wnd_(wnd),
        y_(0),
        height_(1.0f),
        positionUVs_({369, 41, 16, 5})
    {
        visible_ = false;
        Renderer& renderer = Renderer::GetInstance();
        renderer.InitUVs(positionUVs_, Renderer::TEX_GUI);
        Init({351, 351, 366, 366, 40, 40, 46, 46});
        switch (type)
        {
        case OGUI::ScrollBar::VERTICAL:
            arrow1_ = new Button({ 0.0f, 0.0f, 1.0f, 0.1f });
            arrow1_->Init({351, 351, 367, 367, 22, 22, 38, 38});
            AddWidget(arrow1_);
            arrow1_->F = bind(&ScrollBar::ScrollUp, this);
            arrow2_ = new Button({ 0.0f, 0.9f, 1.0f, 0.1f });
            arrow2_->Init({ 351, 351, 367, 367, 38, 38, 22, 22 });
            AddWidget(arrow2_);
            arrow2_->F = bind(&ScrollBar::ScrollDown, this);
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

    void ScrollBar::SetPosition(float y, float height)
    {
        if (height < 0.25f) {
            height = 0.25f;
        }
        if (y + height > 1.0f) {
            y = 1.0f - height;
        }
        y_ = y;
        height_ = height;
    }

    void ScrollBar::Render()
    {
        if (!visible_) {
            return;
        }

        Widget::Render();
        Renderer::GetInstance().SetTexture(Renderer::TEX_GUI);
        Renderer::GetInstance().RenderRect({pos_.left, pos_.top + pos_.height*(0.1f + y_), pos_.width, pos_.height * (height_ - 0.2f)}, positionUVs_);
    }

    void ScrollBar::ScrollUp()
    {
        wnd_->Scroll(0, -0.1f);
    }

    void ScrollBar::ScrollDown()
    {
        wnd_->Scroll(0, 0.1f);
    }

}
