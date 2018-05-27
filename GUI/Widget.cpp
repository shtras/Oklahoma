#include "StdAfx.h"
#include "Widget.h"
#include "MainWindow.h"

namespace OGUI
{
    static int NumWidgets = 0;
    Widget::Widget(Rect pos) :
        pos_(pos),
        relativePos_(pos),
        hovered_(false),
        pressed_(false),
        dragged_(false),
        resized_(false),
        clickable_(false),
        draggable_(false),
        resizable_(false),
        interactive_(true),
        visible_(true),
        keyFocus_(false),
        texState_(NONE),
        dragStartX_(0),
        dragStartY_(0),
        keyboardListener_(false),
        parent_(nullptr)
    {
        ++NumWidgets;
    }

    Widget::~Widget()
    {
        --NumWidgets;
        if (NumWidgets == 0) {
            LogDebug(L"All widgets destroyed");
        }
    }

    void Widget::Init(TexturePos texPos)
    {
        texPos_ = texPos;
        CreateUVs(uvs_, texPos);
        CreateRects();
        texState_ = REGULAR;
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

    void Widget::SetPressedTexture(int pressedX, int pressedY)
    {
        int dx = pressedX - texPos_.l1;
        int dy = pressedY - texPos_.t1;
        TexturePos texPosPressed = { texPos_.l1 + dx, texPos_.l2 + dx, texPos_.l3 + dx, texPos_.l4 + dx,
            texPos_.t1 + dy, texPos_.t2 + dy, texPos_.t3 + dy, texPos_.t4 + dy };
        CreateUVs(pressedUVs_, texPosPressed);
        texState_ |= PRESSED;
    }

    void Widget::CreateUVs(OGraphics::Rect* uvs, TexturePos& texPos)
    {
        Renderer& renderer = Renderer::GetInstance();
        int width = renderer.GetWidth();
        int height = renderer.GetHeight();
        auto tex = renderer.GetTexture(Renderer::TEX_GUI);
        float fl1 = texPos.l1 / (float)tex->GetWidth();
        float fl2 = texPos.l2 / (float)tex->GetWidth();
        float fl3 = texPos.l3 / (float)tex->GetWidth();
        float fl4 = texPos.l4 / (float)tex->GetWidth();

        float ft1 = texPos.t1 / (float)tex->GetHeight();
        float ft2 = texPos.t2 / (float)tex->GetHeight();
        float ft3 = texPos.t3 / (float)tex->GetHeight();
        float ft4 = texPos.t4 / (float)tex->GetHeight();
        uvs[0] = { fl1, ft1, fl2 - fl1, ft2 - ft1 };
        uvs[1] = { fl2, ft1, fl3 - fl2, ft2 - ft1 };
        uvs[2] = { fl3, ft1, fl4 - fl3, ft2 - ft1 };

        uvs[3] = { fl1, ft2, fl2 - fl1, ft3 - ft2 };
        uvs[4] = { fl2, ft2, fl3 - fl2, ft3 - ft2 };
        uvs[5] = { fl3, ft2, fl4 - fl3, ft3 - ft2 };

        uvs[6] = { fl1, ft3, fl2 - fl1, ft4 - ft3 };
        uvs[7] = { fl2, ft3, fl3 - fl2, ft4 - ft3 };
        uvs[8] = { fl3, ft3, fl4 - fl3, ft4 - ft3 };
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
            OGraphics::Rect* uvs = uvs_;
            if (hovered_ && (texState_ & HOVERED)) {
                uvs = hoveredUVs_;
            }
            if (pressed_ && (texState_ & PRESSED)) {
                uvs = pressedUVs_;
            }
            renderer.SetTexture(Renderer::TEX_GUI);
            for (int i = 0; i < 9; ++i) {
                renderer.RenderRect(rects_[i], uvs[i]);
            }
        }
        RenderChildren();
    }

    void Widget::Fit(Rect containingRect)
    {
        pos_.left = containingRect.left + relativePos_.left * containingRect.width;
        pos_.top = containingRect.top + relativePos_.top * containingRect.height;
        pos_.width = relativePos_.width * containingRect.width;
        pos_.height = relativePos_.height * containingRect.height;
        CreateRects();
        for (auto& itr : children_) {
            itr->Fit(pos_);
        }
    }

    void Widget::Resize(float dx, float dy)
    {
        relativePos_.width += dx / parent_->pos_.width;
        relativePos_.height += dy / parent_->pos_.height;
        Fit(parent_->pos_);
    }


    void Widget::Move(float x, float y, float dx, float dy)
    {
        if (!parent_->IsWithin(x, y)) {
            return;
        }
        Move(dx, dy);
    }

    void Widget::Move(float dx, float dy)
    {
        relativePos_.left += dx / parent_->pos_.width;
        relativePos_.top += dy / parent_->pos_.height;
        if (parent_) {
            parent_->OnChildMove(this);
        }
        Fit(parent_->pos_);
    }

    void Widget::RenderChildren()
    {
        for (auto& itr : children_) {
            if (itr->pos_.top > pos_.top + pos_.height) {
                continue;
            }
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
        auto& mainWindow = MainWindow::GetInstance();
        if (draggable_ && parent_) {
            parent_->MoveToTop(this);
        }
        float border = 0.1f;
        if (resizable_) {
            if (x >= pos_.left + pos_.width * (1.0f - border) && y >= pos_.top + pos_.height * (1.0f - border)) {
                mainWindow.RegisterResized(this, MainWindow::ResizeDirection::BottomRight);
                return;
            }
            if (x <= pos_.left + pos_.width * border && y >= pos_.top + pos_.height * (1.0f - border)) {
                mainWindow.RegisterResized(this, MainWindow::ResizeDirection::BottomLeft);
                return;
            }
            if (x >= pos_.left + pos_.width * (1.0f - border) && y <= pos_.top + pos_.height * border) {
                mainWindow.RegisterResized(this, MainWindow::ResizeDirection::TopRight);
                return;
            }
            if (x <= pos_.left + pos_.width * border && y <= pos_.top + pos_.height * border) {
                mainWindow.RegisterResized(this, MainWindow::ResizeDirection::TopLeft);
                return;
            }
            if (x >= pos_.left + pos_.width * (1.0f - border)) {
                mainWindow.RegisterResized(this, MainWindow::ResizeDirection::Right);
                return;
            }
            if (x <= pos_.left + pos_.width * border) {
                mainWindow.RegisterResized(this, MainWindow::ResizeDirection::Left);
                return;
            }
            if (y >= pos_.top + pos_.height * (1.0f - border)) {
                mainWindow.RegisterResized(this, MainWindow::ResizeDirection::Bottom);
                return;
            }
            if (y <= pos_.top + pos_.height * border) {
                mainWindow.RegisterResized(this, MainWindow::ResizeDirection::Top);
                return;
            }
        }
        if (draggable_) {
            mainWindow.RegisterDragged(this);
        } else if (clickable_) {
            mainWindow.RegisterPressed(this);
        }
    }

    void Widget::HandleMouseUp(float x, float y)
    {
        auto& mainWindow = MainWindow::GetInstance();
        mainWindow.RegisterDragged(nullptr);
        mainWindow.RegisterPressed(nullptr);
        mainWindow.RegisterResized(nullptr, MainWindow::ResizeDirection::None);
        if (clickable_) {
            OnClick();
        }
        if (keyboardListener_) {
            mainWindow.RegisterKeyboardListener(this);
        } else {
            mainWindow.RegisterKeyboardListener(nullptr);
        }
    }

    void Widget::SetParent(Widget* w)
    {
        parent_ = w;
    }

    void Widget::MoveToTop(Widget* w)
    {
        std::shared_ptr<Widget> sp;
        for (auto& itr : children_) {
            if (itr.get() == w) {
                sp = itr;
                break;
            }
        }
        children_.remove(sp);
        children_.push_back(sp);
        if (parent_) {
            parent_->MoveToTop(this);
        }
    }

    void Widget::SetDraggable(bool value)
    {
        draggable_ = value;
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

    void Widget::ToggleKeyFocus(bool val)
    {
        keyFocus_ = val;
    }

    void Widget::ToggleResized(bool val)
    {
        resized_ = val;
    }

    void Widget::AddWidget(std::shared_ptr<Widget> widget)
    {
        children_.push_back(widget);
        widget->Fit(pos_);
        widget->SetParent(this);
    }

    bool Widget::HandleMouseEvent(SDL_Event& event, float x, float y)
    {
        if (!interactive_ || !visible_) {
            return false;
        }
        if (!IsWithin(x, y)) {
            if (hovered_) {
                MainWindow::GetInstance().RegisterHovered(nullptr);
            }
            if (pressed_) {
                MainWindow::GetInstance().RegisterPressed(nullptr);
            }
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

    void Widget::SetVisible(bool value)
    {
        visible_ = value;
    }

    void Widget::OnChildMove(Widget* w)
    {

    }

}
