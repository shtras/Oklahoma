#include "StdAfx.h"
#include "Oklahoma.h"
#include "Version.h"
#include "Widget.h"
#include "MainWindow.h"
#include "Window.h"
#include "Label.h"
#include "Button.h"
#include "TextInput.h"

using namespace OGraphics;
using namespace OGUI;

int __stdcall WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
#ifdef DEBUG
    ODiagnostics::Logger::GetInstance().SetSeverity(ODiagnostics::LOG_DEBUG);
    AllocConsole();
    FILE* stream = NULL;
    errno_t err = _wfreopen_s(&stream, L"CON", L"w", stdout);
    std::wcout.clear();
#endif

    LogInfo(L"Entering the matrix. %d.%d.%d %ls built on %hs %hs from %ls", Version::MAJOR, Version::MINOR, Version::REVISION,
#ifdef DEBUG
        L"Debug"
#else
        L"Release"
#endif
        , __DATE__
        , __TIME__
        , GIT_HASH);
    Oklahoma& oklahoma = Oklahoma::GetInstance();
    oklahoma.Init();
    oklahoma.Run();
    LogInfo(L"Bye.\n\n");

#ifdef DEBUG
    FreeConsole();
#endif
    return 0;
}

Oklahoma& Oklahoma::GetInstance()
{
    static Oklahoma instance;
    return instance;
}

void Oklahoma::Run()
{
    int lastTicks = SDL_GetTicks();
    int fpsTickBase = lastTicks;
    int frames = 0;
    wchar_t fpsStr[128];
    Renderer& renderer = Renderer::GetInstance();
    MainWindow& mainWindow = MainWindow::GetInstance();
    
    Window* w = new Window({ 0.1f, 0.1f, 0.5f, 0.5f });
    w->ToggleScrollBar(ScrollBar::VERTICAL, true);
//     Button* btn = new Button({ 0.5f, 0.8f, 0.2f, 0.1f });
//     btn->F = bind(&Oklahoma::Quit, this);
//     btn->SetText(L"OK!");
//     w->AddWidget(btn);

     Window* w1 = new Window({ 0.1f, 0.1f, 0.8f, 0.6f });
     //w1->SetDraggable(false);
     w1->ToggleScrollBar(ScrollBar::VERTICAL, true);
     for (int i = 0; i < 1; ++i) {
         Label* l = new Label({ 0.1f, i * 0.15f, 0.8f, 0.15f });
         wchar_t txt[128];
         swprintf_s(txt, L"Labeliko # %d", i);
         l->SetText(txt);
         w1->AddWidget(l);
     }
     w->AddWidget(w1);

    //SmartPtr<Widget> inp = new TextInput({0.1f, 0.1f, 0.8f, 0.1f});
    //w->AddWidget(inp);

    mainWindow.AddWidget(w);

    Label* fpsText = new Label({ 0.0f, 0.9f, 0.1f, 0.1f });
    mainWindow.AddWidget(fpsText);
    while (running_) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_QUIT:
                running_ = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEMOTION:
            {
                if (event.type == SDL_MOUSEBUTTONUP) {
                    int a = 0;
                }
                float x = event.motion.x / (float)renderer.GetWidth();
                float y = event.motion.y / (float)renderer.GetHeight();
                mainWindow.HandleMouseEvent(event, x, y);
                break;
            }
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                mainWindow.HandleKeyboardEvent(event);
                break;
            }
        }

        int currTicks = SDL_GetTicks();
        int deltaTicks = currTicks - lastTicks;
        ++frames;
        if (deltaTicks > 100) {
            lastTicks = currTicks;
            float fps = frames / (float)deltaTicks * 1000.0f;
            frames = 0;
            swprintf_s(fpsStr, L"FPS: %.2f", fps);
            (fpsText)->SetText(fpsStr);
        }

        renderer.StartFrame();
        
        mainWindow.Render();
        renderer.RenderFrame();
    }
}

void Oklahoma::Init()
{
    Renderer& renderer = Renderer::GetInstance();
    renderer.Init();
    renderer.SetCharSize(20, 40);
}

void Oklahoma::Quit()
{
    running_ = false;
}

Oklahoma::Oklahoma() :
    running_(true)
{

}

Oklahoma::~Oklahoma()
{
}
