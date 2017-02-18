#pragma once
#include "Widget.h"

namespace OGUI
{
    class MainWindow : public Widget
    {
    public:
        static MainWindow& GetInstance();
        void RegisterHovered(Widget* w);
        void RegisterPressed(Widget* w);
        void RegisterDragged(Widget* w);
        bool HandleMouseEvent(SDL_Event& event, float x, float y) override;
    private:
        MainWindow();
        ~MainWindow();
        Widget* hoveredWidget_;
        Widget* pressedWidget_;
        Widget* draggedWidget_;
    };

}
