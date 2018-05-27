#pragma once
#include "Widget.h"

namespace OGUI
{
    class MainWindow : public Widget
    {
    public:
        enum class ResizeDirection {None, TopLeft, Top, TopRight, Right, BottomRight, Bottom, BottomLeft, Left};
        static MainWindow& GetInstance();
        void RegisterHovered(Widget* w);
        void RegisterPressed(Widget* w);
        void RegisterDragged(Widget* w);
        void RegisterResized(Widget* w, ResizeDirection dir);
        void RegisterKeyboardListener(Widget* w);
        bool HandleMouseEvent(SDL_Event& event, float x, float y) override;
        void HandleKeyboardEvent(SDL_Event& event) override;
    private:
        MainWindow() noexcept;
        ~MainWindow();
        Widget* hoveredWidget_;
        Widget* pressedWidget_;
        Widget* draggedWidget_;
        Widget* keyboardListenerWidget_;
        Widget* resizedWidget_;
        ResizeDirection resizeDir_;
        float mx_;
        float my_;
    };

}
