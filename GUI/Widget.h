#pragma once
#include "Renderer.h"
#include "GUIInterface.h"
using namespace OGraphics;

namespace OGUI
{
    class MainWindow;

    class Widget
    {
        enum TextureState
        {
            NONE=0,
            REGULAR=1,
            HOVERED=2,
            PRESSED=4
        };
    public:
        enum class ResizeDirection { None, TopLeft, Top, TopRight, Right, BottomRight, Bottom, BottomLeft, Left };
        enum ParentRelation : unsigned int { PPFloat = 0, PPScaledVertical = 1, PPScaledHorizontal = 2, PPStickRight = 4, PPStickBottom = 8 };
        friend class Window;
        virtual ~Widget();
        virtual void Render();
        virtual void AddWidget(std::shared_ptr<Widget> widget);
        virtual bool HandleMouseEvent(SDL_Event& event, float x, float y);
        void ToggleHovered(bool val);
        void TogglePressed(bool val);
        void ToggleDragged(bool val);
        void ToggleKeyFocus(bool val);
        void ToggleResized(bool val);
        void Init(TexturePos texPos);
        void SetHoveredTexture(int hoveredX, int hoveredY);
        void SetPressedTexture(int pressedX, int pressedY);
        void Move(float x, float y, float dx, float dy);
        void Move(float dx, float dy);
        void Resize(float dx, float dy);
        virtual void HandleKeyboardEvent(SDL_Event& event) { assert(0); };
        void SetDraggable(bool value);
        void SetVisible(bool value);
        void SetParentRelation(unsigned int relation);
        virtual void OnChildMove(Widget* w);
        ResizeDirection GetResizeDirection(float x, float y) const;
    protected:
        Widget(OGraphics::Rect pos, OGUI::MainWindow* mw);
        Widget() = delete;
        Widget(const Widget& other) = delete;
        void createUVs(OGraphics::Rect* uvs, TexturePos& texPos);
        void createRects();
        void fit(Rect containingRect);
        void renderChildren();
        bool isWithin(float x, float y);
        virtual void handleMouseEventSelf(SDL_Event& event, float x, float y);
        virtual void handleMouseDown(float x, float y);
        virtual void handleMouseUp(float x, float y);
        void setParent(Widget* w);
        void moveToTop(Widget* w);
        virtual void OnClick() { };
        virtual void OnResize() {};
        Rect screenPos_;
        Rect relativePos_;
        TexturePos texPos_;
        OGraphics::Rect uvs_[9];
        OGraphics::Rect hoveredUVs_[9];
        OGraphics::Rect pressedUVs_[9];
        OGraphics::Rect rects_[9];
        std::list<std::shared_ptr<Widget>> children_;
        int texState_;
        bool hovered_;
        bool pressed_;
        bool dragged_;
        bool resized_;
        bool visible_;
        bool keyFocus_;
        bool interactive_;

        bool clickable_;
        bool draggable_;
        bool resizable_;
        bool keyboardListener_;
        float dragStartX_;
        float dragStartY_;
        Widget* parent_;
        MainWindow* mainWindow_;
        unsigned int parentRelation_ = PPFloat;
    };
}
