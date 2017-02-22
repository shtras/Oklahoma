#include "StdAfx.h"
#include "Window.h"

namespace OGUI
{

    Window::Window(Rect pos) :
        Widget(pos)
    {
        draggable_ = true;
        Init({2, 36, 328, 333, 2, 36, 225, 240});
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

}
