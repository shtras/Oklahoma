#include "StdAfx.h"
#include "Oklahoma.h"
#include "Version.h"
#include "Widget.h"
#include "MainWindow.h"

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
    TexturePos tex = { 31, 47, 321, 338, 29, 54, 210, 237 };
    SmartPtr<Widget> w = new Widget({ 0.1f, 0.1f, 0.8f, 0.8f });
    w->Init(tex);
    //w->SetHoveredTexture(355, 29);
    SmartPtr<Widget> w1 = new Widget({ 0.1f, 0.1f, 0.3f, 0.3f });
    w1->Init(tex);
    w1->SetHoveredTexture(355, 29);
    SmartPtr<Widget> w2 = new Widget({ 0.1f, 0.6f, 0.3f, 0.3f });
    w2->Init(tex);
    w2->SetHoveredTexture(355, 29);
    SmartPtr<Widget> w3 = new Widget({ 0.1f, 0.1f, 0.8f, 0.8f });
    w3->Init(tex);
    mainWindow.AddWidget(w);
    w->AddWidget(w1);
    w->AddWidget(w2);
    w2->AddWidget(w3);
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
                float x = event.motion.x / (float)renderer.GetWidth();
                float y = event.motion.y / (float)renderer.GetHeight();
                mainWindow.HandleMouseEvent(event, x, y);
                break;
            }
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
        }

        renderer.StartFrame();
        
        renderer.RenderText(fpsStr, 0, 0.9f);
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

Oklahoma::Oklahoma() :
    running_(true)
{

}

Oklahoma::~Oklahoma()
{
}
