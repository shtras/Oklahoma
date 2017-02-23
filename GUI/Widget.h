#pragma once
#include "Renderer.h"
#include "GUIInterface.h"
using namespace OGraphics;

namespace OGUI
{
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
        Widget(Rect pos);
        virtual ~Widget();
        virtual void Render();
        void AddWidget(SmartPtr<Widget> widget);
        virtual bool HandleMouseEvent(SDL_Event& event, float x, float y);
        void ToggleHovered(bool val);
        void TogglePressed(bool val);
        void ToggleDragged(bool val);
        void ToggleKeyFocus(bool val);
        void Init(TexturePos texPos);
        void SetHoveredTexture(int hoveredX, int hoveredY);
        void SetPressedTexture(int pressedX, int pressedY);
        void Move(float x, float y, float dx, float dy);

        virtual void SetText(wstring& str) { assert(0); };
        virtual void SetText(const wchar_t* str) { assert(0); };
        virtual void HandleKeyboardEvent(SDL_Event& event) { assert(0); };
        void SetDraggable(bool value);
        void SetVisible(bool value);
    protected:
        Widget();
        Widget(const Widget& other);
        void CreateUVs(WidgetRects& uvs, TexturePos& texPos);
        void CreateRects();
        void Resize(Rect containingRect);
        void RenderChildren();
        bool IsWithin(float x, float y);
        virtual void HandleMouseEventSelf(SDL_Event& event, float x, float y);
        virtual void HandleMouseDown(float x, float y);
        virtual void HandleMouseUp(float x, float y);
        void SetParent(Widget* w);
        void MoveToTop(Widget* w);
        void Move(float dx, float dy);
        virtual void OnClick() { };
        Rect pos_;
        TexturePos texPos_;
        WidgetRects uvs_;
        WidgetRects hoveredUVs_;
        WidgetRects pressedUVs_;
        OGraphics::Rect rects_[9];
        list<SmartPtr<Widget>> children_;
        int texState_;
        bool hovered_;
        bool pressed_;
        bool dragged_;
        bool visible_;
        bool keyFocus_;
        bool interactive_;

        bool clickable_;
        bool draggable_;
        bool keyboardListener_;
        float dragStartX_;
        float dragStartY_;
        Widget* parent_;
    };
}
