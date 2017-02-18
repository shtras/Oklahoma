#include "StdAfx.h"
#include "MainWindow.h"

namespace OGUI
{
    MainWindow::MainWindow():
        Widget({ 0.0f, 0.0f, 1.0f, 1.0f }, {})
    {

    }

    MainWindow::~MainWindow()
    {

    }

    MainWindow& MainWindow::GetInstance()
    {
        static MainWindow instance;
        return instance;
    }

    void MainWindow::Render()
    {
        RenderChildren();
    }

}
