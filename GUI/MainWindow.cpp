#include "StdAfx.h"
#include "MainWindow.h"
#include "Renderer.h"

namespace OGUI
{
    MainWindow::MainWindow():
        Widget({ 0.0f, 0.0f, 1.0f, 1.0f }),
        hoveredWidget_(nullptr),
        pressedWidget_(nullptr),
        draggedWidget_(nullptr)
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

    bool MainWindow::HandleMouseEvent(SDL_Event& event, float x, float y)
    {
        if (draggedWidget_ && event.type == SDL_MOUSEMOTION) {
            Renderer& renderer = Renderer::GetInstance();
            draggedWidget_->Move(x, y, event.motion.xrel / (float)renderer.GetWidth(), event.motion.yrel / (float)renderer.GetHeight());
            return true;
        }
        return Widget::HandleMouseEvent(event, x, y);
    }

    MainWindow& MainWindow::GetInstance()
    {
        static MainWindow instance;
        return instance;
    }
}
