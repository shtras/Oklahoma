#pragma once
#include "Widget.h"

namespace OGUI
{
    class MainWindow : public Widget
    {
    public:
        static MainWindow& GetInstance();
        void RegisterHovered(Widget* w);
    private:
        MainWindow();
        ~MainWindow();
        Widget* hoveredWidget_;
    };

}
