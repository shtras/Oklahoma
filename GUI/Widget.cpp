#include "StdAfx.h"
#include "Widget.h"
#include "mainWindow.h"

namespace OGUI
{
    static int NumWidgets = 0;
    Widget::Widget(OGraphics::Rect pos, OGUI::MainWindow* mw) :
        screenPos_(pos),
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
        parent_(nullptr),
        mainWindow_(mw)
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
        createUVs(uvs_, texPos);
        createRects();
        texState_ = REGULAR;
    }

    void Widget::SetHoveredTexture(int hoveredX, int hoveredY)
    {
        int dx = hoveredX - texPos_.l1;
        int dy = hoveredY - texPos_.t1;
        TexturePos texPosHovered = { texPos_.l1 + dx, texPos_.l2 + dx, texPos_.l3 + dx, texPos_.l4 + dx,
                                     texPos_.t1 + dy, texPos_.t2 + dy, texPos_.t3 + dy, texPos_.t4 + dy };
        createUVs(hoveredUVs_, texPosHovered);
        texState_ |= HOVERED;
    }

    void Widget::SetPressedTexture(int pressedX, int pressedY)
    {
        int dx = pressedX - texPos_.l1;
        int dy = pressedY - texPos_.t1;
        TexturePos texPosPressed = { texPos_.l1 + dx, texPos_.l2 + dx, texPos_.l3 + dx, texPos_.l4 + dx,
            texPos_.t1 + dy, texPos_.t2 + dy, texPos_.t3 + dy, texPos_.t4 + dy };
        createUVs(pressedUVs_, texPosPressed);
        texState_ |= PRESSED;
    }

    void Widget::createUVs(OGraphics::Rect* uvs, TexturePos& texPos)
    {
        Renderer& renderer = Renderer::GetInstance();
        int width = renderer.GetWidth();
        int height = renderer.GetHeight();
        auto tex = renderer.GetTexture(Renderer::TextureType::GUI);
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

    void Widget::createRects()
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
        float fw2 = screenPos_.width - fw1 - fw3;
        float fh1 = h1 / (float)height;
        float fh3 = h3 / (float)height;
        float fh2 = screenPos_.height - fh1 - fh3;
        rects_[0] = { screenPos_.left, screenPos_.top, fw1, fh1 };
        rects_[1] = { screenPos_.left + fw1, screenPos_.top, fw2, fh1 };
        rects_[2] = { screenPos_.left + fw1 + fw2, screenPos_.top, fw3, fh1 };
        rects_[3] = { screenPos_.left, screenPos_.top + fh1, fw1, fh2 };
        rects_[4] = { screenPos_.left + fw1, screenPos_.top + fh1, fw2, fh2 };
        rects_[5] = { screenPos_.left + fw1 + fw2, screenPos_.top + fh1, fw3, fh2 };
        rects_[6] = { screenPos_.left, screenPos_.top + fh1 + fh2, fw1, fh3 };
        rects_[7] = { screenPos_.left + fw1, screenPos_.top + fh1 + fh2, fw2, fh3 };
        rects_[8] = { screenPos_.left + fw1 + fw2, screenPos_.top + fh1 + fh2, fw3, fh3 };
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
            renderer.SetTexture(Renderer::TextureType::GUI);
            for (int i = 0; i < 9; ++i) {
                renderer.RenderRect(rects_[i], uvs[i]);
            }
        }
        renderChildren();
    }

    void Widget::fit(Rect containingRect)
    {
        screenPos_.left = containingRect.left + relativePos_.left * containingRect.width;
        screenPos_.top = containingRect.top + relativePos_.top * containingRect.height;
        screenPos_.width = relativePos_.width * containingRect.width;
        screenPos_.height = relativePos_.height * containingRect.height;
        createRects();
        for (auto& itr : children_) {
            itr->fit(screenPos_);
        }
    }

    void Widget::Resize(float dx, float dy)
    {
        float prevRelWidth = relativePos_.width;
        float prevRelHeight = relativePos_.height;
        relativePos_.width += dx / parent_->screenPos_.width;
        relativePos_.height += dy / parent_->screenPos_.height;
        float dW = prevRelWidth / relativePos_.width;
        float dH = prevRelHeight / relativePos_.height;
        for (auto& itr : children_) {
            if (!(itr->parentRelation_ & PPStickRight)) {
                itr->relativePos_.left *= dW;
            } else {
                float distFromRight = 1.0f - itr->relativePos_.left - itr->relativePos_.width;
                distFromRight *= dW;
                itr->relativePos_.left = 1.0f - distFromRight - itr->relativePos_.width * dW;
            }
            if (!(itr->parentRelation_ & PPStickBottom)) {
                itr->relativePos_.top *= dH;
            }
            itr->relativePos_.width *= dW;
            if (!(itr->parentRelation_ & PPScaledVertical)) {
                itr->relativePos_.height *= dH;
            }
        }
        fit(parent_->screenPos_);
    }

    void Widget::Move(float x, float y, float dx, float dy)
    {
        if (!parent_->isWithin(x, y)) {
            return;
        }
        Move(dx, dy);
    }

    void Widget::Move(float dx, float dy)
    {
        relativePos_.left += dx / parent_->screenPos_.width;
        relativePos_.top += dy / parent_->screenPos_.height;
        if (parent_) {
            parent_->OnChildMove(this);
        }
        fit(parent_->screenPos_);
    }

    void Widget::renderChildren()
    {
        for (auto& itr : children_) {
            if (itr->screenPos_.top > screenPos_.top + screenPos_.height) {
                continue;
            }
            itr->Render();
        }
    }

    bool Widget::isWithin(float x, float y)
    {
        if (x < screenPos_.left || x > screenPos_.left + screenPos_.width) {
            return false;
        }
        if (y < screenPos_.top || y > screenPos_.top + screenPos_.height) {
            return false;
        }
        return true;
    }

    void Widget::handleMouseEventSelf(SDL_Event& event, float x, float y)
    {
        switch (event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            handleMouseDown(x, y);
            break;
        case SDL_MOUSEBUTTONUP:
            handleMouseUp(x, y);
            break;
        default:
            break;
        }
    }

    Widget::ResizeDirection Widget::GetResizeDirection(float x, float y) const
    {
        float border = 0.1f;
        if (x >= screenPos_.left + screenPos_.width * (1.0f - border) && y >= screenPos_.top + screenPos_.height * (1.0f - border)) {
            return ResizeDirection::BottomRight;
        }
        if (x <= screenPos_.left + screenPos_.width * border && y >= screenPos_.top + screenPos_.height * (1.0f - border)) {
            return ResizeDirection::BottomLeft;
        }
        if (x >= screenPos_.left + screenPos_.width * (1.0f - border) && y <= screenPos_.top + screenPos_.height * border) {
            return ResizeDirection::TopRight;
        }
        if (x <= screenPos_.left + screenPos_.width * border && y <= screenPos_.top + screenPos_.height * border) {
            return ResizeDirection::TopLeft;
        }
        if (x >= screenPos_.left + screenPos_.width * (1.0f - border)) {
            return ResizeDirection::Right;
        }
        if (x <= screenPos_.left + screenPos_.width * border) {
            return ResizeDirection::Left;
        }
        if (y >= screenPos_.top + screenPos_.height * (1.0f - border)) {
            return ResizeDirection::Bottom;
        }
        if (y <= screenPos_.top + screenPos_.height * border) {
            return ResizeDirection::Top;
        }
        return ResizeDirection::None;
    }

    void Widget::handleMouseDown(float x, float y)
    {
        if (draggable_ && parent_) {
            parent_->moveToTop(this);
        }
        if (resizable_) {
            auto resizeDir = GetResizeDirection(x, y);
            if (resizeDir != ResizeDirection::None) {
                mainWindow_->RegisterResized(this, resizeDir);
                return;
            }
        }
        if (draggable_) {
            mainWindow_->RegisterDragged(this);
        } else if (clickable_) {
            mainWindow_->RegisterPressed(this);
        }
    }

    void Widget::handleMouseUp(float x, float y)
    {
        mainWindow_->RegisterDragged(nullptr);
        mainWindow_->RegisterPressed(nullptr);
        mainWindow_->RegisterResized(nullptr, MainWindow::ResizeDirection::None);
        if (clickable_) {
            onClick();
        }
        if (keyboardListener_) {
            mainWindow_->RegisterKeyboardListener(this);
        } else {
            mainWindow_->RegisterKeyboardListener(nullptr);
        }
    }

    void Widget::setParent(Widget* w)
    {
        parent_ = w;
    }

    void Widget::moveToTop(Widget* w)
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
            parent_->moveToTop(this);
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
        widget->fit(screenPos_);
        widget->setParent(this);
    }

    bool Widget::HandleMouseEvent(SDL_Event& event, float x, float y)
    {
        if (!interactive_ || !visible_) {
            return false;
        }
        if (!isWithin(x, y)) {
            if (hovered_) {
                mainWindow_->RegisterHovered(nullptr);
            }
            if (pressed_) {
                mainWindow_->RegisterPressed(nullptr);
            }
            return false;
        }
        for (auto& itr = children_.rbegin(); itr != children_.rend(); ++itr) {
            if ((*itr)->HandleMouseEvent(event, x, y)) {
                return true;
            }
        }

        mainWindow_->RegisterHovered(this);

        auto& renderer = Renderer::GetInstance();
        if (resizable_) {
            ResizeDirection resizeDir = GetResizeDirection(x, y);
            switch (resizeDir)
            {
            case OGUI::Widget::ResizeDirection::TopLeft:
            case OGUI::Widget::ResizeDirection::BottomRight:
                renderer.SetMouseCursor(Renderer::MouseCursor::ResizeDiagonalNW);
                break;
            case OGUI::Widget::ResizeDirection::Top:
            case OGUI::Widget::ResizeDirection::Bottom:
                renderer.SetMouseCursor(Renderer::MouseCursor::ResizeVertical);
                break;
            case OGUI::Widget::ResizeDirection::TopRight:
            case OGUI::Widget::ResizeDirection::BottomLeft:
                renderer.SetMouseCursor(Renderer::MouseCursor::ResizeDiagonalNE);
                break;
            case OGUI::Widget::ResizeDirection::Right:
            case OGUI::Widget::ResizeDirection::Left:
                renderer.SetMouseCursor(Renderer::MouseCursor::ResizeHorizontal);
                break;
            default:
                renderer.SetMouseCursor(Renderer::MouseCursor::Default);
                break;
            }
        } else {
            renderer.SetMouseCursor(Renderer::MouseCursor::Default);
        }

        handleMouseEventSelf(event, x, y);
        return true;
    }

    void Widget::SetVisible(bool value)
    {
        visible_ = value;
    }

    void Widget::SetParentRelation(unsigned int relation)
    {
        parentRelation_ = relation;
    }

    void Widget::OnChildMove(Widget* w)
    {

    }
}
