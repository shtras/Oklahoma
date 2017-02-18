#pragma once
#include "Widget.h"

namespace OGUI
{
    class MainWindow : public Widget
    {
    public:
        static MainWindow& GetInstance();
        void Render();
    private:
        MainWindow();
        ~MainWindow();
    };

}
