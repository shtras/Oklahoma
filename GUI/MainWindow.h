#pragma once
#include "Widget.h"

namespace OGUI
{
    class MainWindow : public Widget
    {
    public:
        MainWindow() noexcept;
        ~MainWindow();
        
        template <class T, class... _Types>
        std::shared_ptr<T> CreateWidget(_Types&&... _Args) {
            auto res = std::make_shared<T>(_STD forward<_Types>(_Args)..., this);
            return res;
        }

        void RegisterHovered(Widget* w);
        void RegisterPressed(Widget* w);
        void RegisterDragged(Widget* w);
        void RegisterResized(Widget* w, ResizeDirection dir);
        void RegisterKeyboardListener(Widget* w);
        bool HandleMouseEvent(SDL_Event& event, float x, float y) override;
        void HandleKeyboardEvent(SDL_Event& event) override;
    private:
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
