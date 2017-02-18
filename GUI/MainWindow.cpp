#include "StdAfx.h"
#include "MainWindow.h"

namespace OGUI
{
    MainWindow::MainWindow():
        Widget({ 0.0f, 0.0f, 1.0f, 1.0f }),
        hoveredWidget_(nullptr)
    {

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
            w->SetHovered(true);
        }
        if (hoveredWidget_) {
            hoveredWidget_->SetHovered(false);
        }
        hoveredWidget_ = w;
    }

    MainWindow& MainWindow::GetInstance()
    {
        static MainWindow instance;
        return instance;
    }
}
