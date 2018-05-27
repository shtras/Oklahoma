#include "StdAfx.h"
#include "MainWindow.h"
#include "Renderer.h"

namespace OGUI
{
    MainWindow::MainWindow() noexcept :
        Widget({ 0.0f, 0.0f, 1.0f, 1.0f }),
        hoveredWidget_(nullptr),
        pressedWidget_(nullptr),
        draggedWidget_(nullptr),
        keyboardListenerWidget_(nullptr),
        mx_(0),
        my_(0)
    {
        clickable_ = false;
        draggable_ = false;
    }

    MainWindow::~MainWindow()
    {

    }

    void MainWindow::RegisterHovered(Widget* w)
    {
        if (w == hoveredWidget_) {
            return;
        }
        if (w) {
            w->ToggleHovered(true);
        }
        if (hoveredWidget_) {
            hoveredWidget_->ToggleHovered(false);
        }
        hoveredWidget_ = w;
    }

    void MainWindow::RegisterPressed(Widget* w)
    {
        if (w == pressedWidget_) {
            return;
        }
        if (w) {
            w->TogglePressed(true);
        }
        if (pressedWidget_) {
            pressedWidget_->TogglePressed(false);
        }
        pressedWidget_ = w;
    }

    void MainWindow::RegisterDragged(Widget* w)
    {
        if (w == draggedWidget_) {
            return;
        }
        if (w) {
            w->ToggleDragged(true);
        }
        if (draggedWidget_) {
            draggedWidget_->ToggleDragged(false);
        }
        draggedWidget_ = w;
    }

    void MainWindow::RegisterResized(Widget* w, ResizeDirection dir)
    {
        if (w == resizedWidget_) {
            return;
        }
        if (w) {
            w->ToggleResized(true);
        }
        if (resizedWidget_) {
            resizedWidget_->ToggleResized(false);
        }
        resizedWidget_ = w;
        resizeDir_ = dir;
    }

    void MainWindow::RegisterKeyboardListener(Widget* w)
    {
        if (w == keyboardListenerWidget_) {
            return;
        }
        if (w) {
            w->ToggleKeyFocus(true);
        }
        if (keyboardListenerWidget_) {
            keyboardListenerWidget_->ToggleKeyFocus(false);
        }
        keyboardListenerWidget_ = w;
    }

    bool MainWindow::HandleMouseEvent(SDL_Event& event, float x, float y)
    {
        auto& renderer = Renderer::GetInstance();
        if (event.type == SDL_MOUSEMOTION) {
            mx_ = x;
            my_ = y;
            if (draggedWidget_) {
                draggedWidget_->Move(x, y, event.motion.xrel / (float)renderer.GetWidth(), event.motion.yrel / (float)renderer.GetHeight());
                return true;
            }
            if (resizedWidget_) {
                switch (resizeDir_)
                {
                case OGUI::MainWindow::ResizeDirection::None:
                    break;
                case OGUI::MainWindow::ResizeDirection::TopLeft:
                    resizedWidget_->Resize(-event.motion.xrel / (float)renderer.GetWidth(), -event.motion.yrel / (float)renderer.GetHeight());
                    resizedWidget_->Move(x, y, event.motion.xrel / (float)renderer.GetWidth(), event.motion.yrel / (float)renderer.GetHeight());
                    break;
                case OGUI::MainWindow::ResizeDirection::Top:
                    resizedWidget_->Resize(0, -event.motion.yrel / (float)renderer.GetHeight());
                    resizedWidget_->Move(x, y, 0, event.motion.yrel / (float)renderer.GetHeight());
                    break;
                case OGUI::MainWindow::ResizeDirection::TopRight:
                    resizedWidget_->Resize(event.motion.xrel / (float)renderer.GetWidth(), -event.motion.yrel / (float)renderer.GetHeight());
                    resizedWidget_->Move(x, y, 0, event.motion.yrel / (float)renderer.GetHeight());
                    break;
                case OGUI::MainWindow::ResizeDirection::Right:
                    resizedWidget_->Resize(event.motion.xrel / (float)renderer.GetWidth(), 0);
                    break;
                case OGUI::MainWindow::ResizeDirection::BottomRight:
                    resizedWidget_->Resize(event.motion.xrel / (float)renderer.GetWidth(), event.motion.yrel / (float)renderer.GetHeight());
                    break;
                case OGUI::MainWindow::ResizeDirection::Bottom:
                    resizedWidget_->Resize(0, event.motion.yrel / (float)renderer.GetHeight());
                    break;
                case OGUI::MainWindow::ResizeDirection::BottomLeft:
                    resizedWidget_->Resize(-event.motion.xrel / (float)renderer.GetWidth(), event.motion.yrel / (float)renderer.GetHeight());
                    resizedWidget_->Move(x, y, event.motion.xrel / (float)renderer.GetWidth(), 0);
                    break;
                case OGUI::MainWindow::ResizeDirection::Left:
                    resizedWidget_->Resize(-event.motion.xrel / (float)renderer.GetWidth(), 0);
                    resizedWidget_->Move(x, y, event.motion.xrel / (float)renderer.GetWidth(), 0);
                    break;
                default:
                    break;
                }
                return true;
            }
        }
        if (event.type == SDL_MOUSEWHEEL) {
            x = mx_;
            y = my_;
        }
        return Widget::HandleMouseEvent(event, x, y);
    }

    void MainWindow::HandleKeyboardEvent(SDL_Event& event)
    {
        if (keyboardListenerWidget_) {
            keyboardListenerWidget_->HandleKeyboardEvent(event);
        }
    }

    MainWindow& MainWindow::GetInstance()
    {
        static MainWindow instance;
        return instance;
    }
}
