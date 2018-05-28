#include "StdAfx.h"
#include "Oklahoma.h"
#include "Version.h"
#include "Window.h"
#include "Label.h"
#include "Button.h"
#include "TextInput.h"
#include "mainWindow.h"

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

    LogInfo(L"Entering the matrix. %d.%d.%d %ls built on %hs %hs from %ls on %ls", Version::MAJOR, Version::MINOR, Version::REVISION,
#ifdef DEBUG
        L"Debug"
#else
        L"Release"
#endif
        , __DATE__, __TIME__, GIT_HASH, GIT_BRANCH);
    auto oklahoma = std::make_shared<Oklahoma>();
    oklahoma->Init();
    oklahoma->Run();
    LogInfo(L"Bye.");

#ifdef DEBUG
    FreeConsole();
#endif
    return 0;
}

void Oklahoma::perfTest()
{
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            auto w = mainWindow_->CreateWidget<Window>(Rect({ i*0.05f, j*0.05f, 0.05f, 0.05f }));
            mainWindow_->AddWidget(w);
            std::shared_ptr<Window> wItr = w;
            std::shared_ptr<Window> wItrNext = nullptr;
            for (int k = 0; k < 5; ++k) {
                wItrNext = mainWindow_->CreateWidget<Window>(Rect({ 0.1f, 0.1f, 0.8f, 0.8f }));
                wItr->AddWidget(wItrNext);
                wItr = wItrNext;
            }
            auto b = mainWindow_->CreateWidget<Button>(Rect({ 0.1f, 0.9f, 0.8f, 0.1f }));
            b->SetText(L"Hello!");
            wItr->AddWidget(b);
        }
    }
}

void Oklahoma::regularTest()
{
    //     auto w00 = mainWindow_->CreateWidget<Window>(Rect({ 0.0f, 0.0f, 1.0f, 1.0f }));
    //     mainWindow_->AddWidget(w00);

    auto w = mainWindow_->CreateWidget<Window>(Rect({ 0.1f, 0.1f, 0.8f, 0.8f }));
    //     w->ToggleScrollBar(ScrollBar::VERTICAL, true);
    //     auto btn = mainWindow_->CreateWidget<Button>(Rect({ 0.4f, 0.8f, 0.2f, 0.1f }));
    //     btn->F = std::bind(&Oklahoma::Quit, this);
    //     btn->SetText(L"OK!");
    //     w->AddWidget(btn);

    auto w0 = mainWindow_->CreateWidget<Window>(Rect({ 0.1f, 0.3f, 0.5f, 0.4f }));
    w->AddWidget(w0);

    auto w1 = mainWindow_->CreateWidget<Window>(Rect({ 0.6f, 0.3f, 0.2f, 0.4f }));
    w1->ToggleScrollBar(ScrollBar::VERTICAL, true);
     for (int i = 0; i < 15; ++i) {
         auto l = mainWindow_->CreateWidget<Label>(Rect({ 0.1f, i * 0.15f, 0.8f, 0.15f }));
         wchar_t txt[128];
         swprintf_s(txt, L"Labeliko # %d", i);
         l->SetText(txt);
         w1->AddWidget(l);
     }
    w->AddWidget(w1);

    auto inp = mainWindow_->CreateWidget<TextInput>(Rect({0.1f, 0.1f, 0.8f, 0.5f}));
    inp->SetMultiline(true);
    w->AddWidget(inp);

    auto inp1 = mainWindow_->CreateWidget<TextInput>(Rect({ 0.1f, 0.7f, 0.8f, 0.1f }));
    w->AddWidget(inp1);

    mainWindow_->AddWidget(w);
}

void Oklahoma::Run()
{
    int lastTicks = SDL_GetTicks();
    int fpsTickBase = lastTicks;
    int frames = 0;
    wchar_t fpsStr[128];
    auto& renderer = Renderer::GetInstance();
    
    regularTest();
    //PerfTest();
    auto fpsText = mainWindow_->CreateWidget<Label>(Rect({ 0.0f, 0.9f, 0.1f, 0.1f }));
    mainWindow_->AddWidget(fpsText);
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
            case SDL_MOUSEWHEEL:
            {
                float x = event.motion.x / (float)renderer.GetWidth();
                float y = event.motion.y / (float)renderer.GetHeight();
                mainWindow_->HandleMouseEvent(event, x, y);
                break;
            }
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                mainWindow_->HandleKeyboardEvent(event);
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
        
        mainWindow_->Render();
        renderer.RenderFrame();
    }
}

void Oklahoma::Init()
{
    Renderer& renderer = Renderer::GetInstance();
    renderer.Init();
    renderer.SetCharSize(20, 40);

    mainWindow_ = std::make_unique<MainWindow>();
}

void Oklahoma::Quit()
{
    running_ = false;
}

Oklahoma::Oklahoma() noexcept :
    running_(true)
{

}

Oklahoma::~Oklahoma()
{
}
