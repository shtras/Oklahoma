#include "StdAfx.h"
#include "Widget.h"
#include "MainWindow.h"

namespace OGUI
{

    Widget::Widget(Rect pos):
        pos_(pos),
        hovered_(false),
        pressed_(false),
        dragged_(false),
        clickable_(false),
        draggable_(true),
        visible_(true),
        texState_(NONE),
        dragStartX_(0),
        dragStartY_(0),
        parent_(nullptr)
    {
    }

    Widget::~Widget()
    {
    }

    void Widget::Init(TexturePos texPos)
    {
        texPos_ = texPos;
        CreateUVs(uvs_, texPos);
        CreateRects();
        texState_ |= REGULAR;
    }

    void Widget::SetHoveredTexture(int hoveredX, int hoveredY)
    {
        int dx = hoveredX - texPos_.l1;
        int dy = hoveredY - texPos_.t1;
        TexturePos texPosHovered = { texPos_.l1 + dx, texPos_.l2 + dx, texPos_.l3 + dx, texPos_.l4 + dx,
                                     texPos_.t1 + dy, texPos_.t2 + dy, texPos_.t3 + dy, texPos_.t4 + dy };
        CreateUVs(hoveredUVs_, texPosHovered);
        texState_ |= HOVERED;
    }

    void Widget::CreateUVs(WidgetRects& uvs, TexturePos& texPos)
    {
        Renderer& renderer = Renderer::GetInstance();
        int width = renderer.GetWidth();
        int height = renderer.GetHeight();
        const Texture* tex = renderer.GetTexture(Renderer::TEX_GUI);
        float fl1 = texPos.l1 / (float)tex->GetWidth();
        float fl2 = texPos.l2 / (float)tex->GetWidth();
        float fl3 = texPos.l3 / (float)tex->GetWidth();
        float fl4 = texPos.l4 / (float)tex->GetWidth();

        float ft1 = texPos.t1 / (float)tex->GetHeight();
        float ft2 = texPos.t2 / (float)tex->GetHeight();
        float ft3 = texPos.t3 / (float)tex->GetHeight();
        float ft4 = texPos.t4 / (float)tex->GetHeight();
        uvs.topLeft = { fl1, ft1, fl2 - fl1, ft2 - ft1 };
        uvs.top = { fl2, ft1, fl3 - fl2, ft2 - ft1 };
        uvs.topRight = { fl3, ft1, fl4 - fl3, ft2 - ft1 };

        uvs.left = { fl1, ft2, fl2 - fl1, ft3 - ft2 };
        uvs.center = { fl2, ft2, fl3 - fl2, ft3 - ft2 };
        uvs.right = { fl3, ft2, fl4 - fl3, ft3 - ft2 };

        uvs.bottomLeft = { fl1, ft3, fl2 - fl1, ft4 - ft3 };
        uvs.bottom = { fl2, ft3, fl3 - fl2, ft4 - ft3 };
        uvs.bottomRight = { fl3, ft3, fl4 - fl3, ft4 - ft3 };
    }

    void Widget::CreateRects()
    {
        Renderer& renderer = Renderer::GetInstance();
        int width = renderer.GetWidth();
        int height = renderer.GetHeight();
        int w1 = texPos_.l2 - texPos_.l1;
        int w2 = texPos_.l3 - texPos_.l2;
        int w3 = texPos_.l4 - texPos_.l3;
        int h1 = texPos_.t2 - texPos_.t1;
        int h2 = texPos_.t3 - texPos_.t2;
        int h3 = texPos_.t4 - texPos_.t3;
        float fw1 = w1 / (float)width;
        float fw3 = w3 / (float)width;
        float fw2 = pos_.width - fw1 - fw3;
        float fh1 = h1 / (float)height;
        float fh3 = h3 / (float)height;
        float fh2 = pos_.height - fh1 - fh3;
        rects_[0] = { pos_.left, pos_.top, fw1, fh1 };
        rects_[1] = { pos_.left + fw1, pos_.top, fw2, fh1 };
        rects_[2] = { pos_.left + fw1 + fw2, pos_.top, fw3, fh1 };
        rects_[3] = { pos_.left, pos_.top + fh1, fw1, fh2 };
        rects_[4] = { pos_.left + fw1, pos_.top + fh1, fw2, fh2 };
        rects_[5] = { pos_.left + fw1 + fw2, pos_.top + fh1, fw3, fh2 };
        rects_[6] = { pos_.left, pos_.top + fh1 + fh2, fw1, fh3 };
        rects_[7] = { pos_.left + fw1, pos_.top + fh1 + fh2, fw2, fh3 };
        rects_[8] = { pos_.left + fw1 + fw2, pos_.top + fh1 + fh2, fw3, fh3 };
    }

    void Widget::Render()
    {
        if (!visible_) {
            return;
        }
        Renderer& renderer = Renderer::GetInstance();
        if (texState_ != NONE) {
            WidgetRects* uvs = &uvs_;
            if (hovered_ && (texState_ & HOVERED)) {
                uvs = &hoveredUVs_;
            }
            renderer.SetTexture(Renderer::TEX_GUI);
            renderer.RenderRect(rects_[0], uvs->topLeft);
            renderer.RenderRect(rects_[1], uvs->top);
            renderer.RenderRect(rects_[2], uvs->topRight);
            renderer.RenderRect(rects_[3], uvs->left);
            renderer.RenderRect(rects_[4], uvs->center);
            renderer.RenderRect(rects_[5], uvs->right);
            renderer.RenderRect(rects_[6], uvs->bottomLeft);
            renderer.RenderRect(rects_[7], uvs->bottom);
            renderer.RenderRect(rects_[8], uvs->bottomRight);
        }
        RenderChildren();
    }

    void Widget::Resize(Rect containingRect)
    {
        pos_.left = containingRect.left + pos_.left * containingRect.width;
        pos_.top = containingRect.top + pos_.top * containingRect.width;
        pos_.width *= containingRect.width;
        pos_.height *= containingRect.height;
        CreateRects();
    }

    void Widget::RenderChildren()
    {
        for (auto& itr : children_) {
            itr->Render();
        }
    }

    bool Widget::IsWithin(float x, float y)
    {
        if (x < pos_.left || x > pos_.left + pos_.width) {
            return false;
        }
        if (y < pos_.top || y > pos_.top + pos_.height) {
            return false;
        }
        return true;
    }

    void Widget::HandleMouseEventSelf(SDL_Event& event, float x, float y)
    {
        switch (event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            HandleMouseDown(x, y);
            break;
        case SDL_MOUSEBUTTONUP:
            HandleMouseUp(x, y);
            break;
        default:
            break;
        }
    }

    void Widget::HandleMouseDown(float x, float y)
    {
        if (draggable_ && parent_) {
            parent_->MoveToTop(this);
        }
        if (draggable_) {
            MainWindow::GetInstance().RegisterDragged(this);
        } else if (clickable_) {
            MainWindow::GetInstance().RegisterPressed(this);
        }
    }

    void Widget::HandleMouseUp(float x, float y)
    {
        MainWindow::GetInstance().RegisterDragged(nullptr);
        MainWindow::GetInstance().RegisterPressed(nullptr);
    }

    void Widget::SetParent(Widget* w)
    {
        parent_ = w;
    }

    void Widget::MoveToTop(Widget* w)
    {
        SmartPtr<Widget> sp;
        for (auto& itr : children_) {
            if (itr == w) {
                sp = itr;
            }
        }
        children_.remove(sp);
        children_.push_back(sp);
    }

    void Widget::Move(float x, float y, float dx, float dy)
    {
        if (!parent_->IsWithin(pos_.left + dx, pos_.top + dy)) {
            return;
        }
        if (!parent_->IsWithin(pos_.left + pos_.width + dx, pos_.top + pos_.height + dy)) {
            return;
        }
        Move(dx, dy);
    }

    void Widget::Move(float dx, float dy)
    {
        for (int i = 0; i < 9; ++i) {
            rects_[i].left += dx;
            rects_[i].top += dy;
        }
        pos_.left += dx;
        pos_.top += dy;
        for (auto& itr : children_) {
            itr->Move(dx, dy);
        }
    }

    void Widget::ToggleHovered(bool val)
    {
        hovered_ = val;
    }

    void Widget::TogglePressed(bool val)
    {
        pressed_ = val;
    }

    void Widget::ToggleDragged(bool val)
    {
        dragged_ = val;
    }

    void Widget::AddWidget(SmartPtr<Widget> widget)
    {
        children_.push_back(widget);
        widget->Resize(pos_);
        widget->SetParent(this);
    }

    bool Widget::HandleMouseEvent(SDL_Event& event, float x, float y)
    {
        if (!IsWithin(x, y)) {
            return false;
        }
        for (auto& itr = children_.rbegin(); itr != children_.rend(); ++itr) {
            if ((*itr)->HandleMouseEvent(event, x, y)) {
                return true;
            }
        }
        MainWindow::GetInstance().RegisterHovered(this);
        HandleMouseEventSelf(event, x, y);
        return true;
    }

}
