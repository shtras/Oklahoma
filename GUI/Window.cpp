#include "StdAfx.h"
#include "Window.h"

namespace OGUI
{

    Window::Window(Rect pos) :
        Widget(pos),
        totalHeight_(1.0f),
        scrollTop_(0),
        verticalScroll_(false),
        horizontalScroll_(false)
    {
        draggable_ = true;
        Init({2, 62, 76, 82, 2, 36, 225, 240});
        SetHoveredTexture(85, 2);
        Renderer& renderer = Renderer::GetInstance();
        float scrollWidth = 50 / (float)renderer.GetWidth();
        float scrollHeight = 50 / (float)renderer.GetHeight();
        scrollBars_[ScrollBar::VERTICAL] = std::make_shared<ScrollBar>(this, Rect({1.0f - scrollWidth - 2 * renderer.GetPixelWidth(), 2 * renderer.GetPixelHeight(), scrollWidth, 1.0f - scrollHeight}), ScrollBar::VERTICAL);
        scrollBars_[ScrollBar::HORIZONTAL] = std::make_shared<ScrollBar>(this, Rect({0.0f, 1.0f - scrollHeight, 1.0f - scrollWidth, scrollHeight}), ScrollBar::HORIZONTAL);
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
        switch (type)
        {
        case OGUI::ScrollBar::VERTICAL:
            verticalScroll_ = val;
            break;
        case OGUI::ScrollBar::HORIZONTAL:
            horizontalScroll_ = val;
            break;
        default:
            assert(0);
            break;
        }
        RecalcContents();
    }

    void Window::AddWidget(std::shared_ptr<Widget> widget)
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
        if (totalHeight_ > 1.0f) {
            scrollBars_[ScrollBar::VERTICAL]->SetVisible(verticalScroll_);
            scrollBars_[ScrollBar::VERTICAL]->SetPosition(scrollTop_ / totalHeight_, 1.0f / totalHeight_);
        } else {
            scrollBars_[ScrollBar::VERTICAL]->SetVisible(false);
            if (scrollTop_ > 0) {
                Scroll(0, scrollTop_);
            }
        }
    }

    void Window::OnChildMove(Widget* w)
    {
        RecalcContents();
    }

    void Window::HandleMouseEventSelf(SDL_Event& event, float x, float y)
    {
        switch (event.type)
        {
        case SDL_MOUSEWHEEL:
            Scroll(0, -event.wheel.y / 10.0f);
            break;
        default:
            Widget::HandleMouseEventSelf(event, x, y);
            break;
        }
    }

    ScrollBar::ScrollBar(Window* wnd, Rect pos, ScrollBarType type) :
        Widget(pos),
        wnd_(wnd),
        y_(0),
        height_(1.0f),
        positionUVs_({201, 41, 16, 5})
    {
        visible_ = false;
        clickable_ = true;
        Renderer& renderer = Renderer::GetInstance();
        renderer.InitUVs(positionUVs_, Renderer::TEX_GUI);
        Init({ 183, 186, 197, 199, 40, 40, 46, 46 });
        switch (type)
        {
        case OGUI::ScrollBar::VERTICAL:
            arrow1_ = std::make_shared<Button>(Rect({ 0.0f, 0.0f, 1.0f, 0.1f }));
            arrow1_->Init({ 183, 185, 196, 199, 22, 22, 38, 38 });
            AddWidget(arrow1_);
            arrow1_->F = std::bind(&ScrollBar::ScrollUp, this);
            arrow2_ = std::make_shared<Button>(Rect({ 0.0f, 0.9f, 1.0f, 0.1f }));
            arrow2_->Init({ 183, 185, 196, 199, 38, 38, 22, 22 });
            AddWidget(arrow2_);
            arrow2_->F = std::bind(&ScrollBar::ScrollDown, this);
            break;
        case OGUI::ScrollBar::HORIZONTAL:
            arrow1_ = std::make_shared<Button>(Rect({ 0.0f, 0.0f, 0.1f, 1.0f }));
            arrow1_->Init({ 201, 203, 214, 216, 22, 22, 38, 38 });
            AddWidget(arrow1_);
            arrow2_ = std::make_shared<Button>(Rect({ 0.9f, 0.0f, 0.1f, 1.0f }));
            arrow2_->Init({ 216, 214, 203, 201, 22, 22, 38, 38 });
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

    void ScrollBar::HandleMouseEventSelf(SDL_Event& event, float x, float y)
    {
        Renderer& renderer = Renderer::GetInstance();
        switch (event.type)
        {
        case SDL_MOUSEMOTION:
            if (pressed_) {
                float dy = event.motion.yrel / (float)renderer.GetHeight();
                wnd_->Scroll(0, dy / height_ / pos_.height);
            }
            break;
        default:
            Widget::HandleMouseEventSelf(event, x, y);
            break;
        }
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
