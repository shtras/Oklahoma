#include "StdAfx.h"
#include "Oklahoma.h"
#include "Version.h"
#include "Widget.h"

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

    LogInfo(L"Entering the matrix. %d.%d.%d", Version::MAJOR, Version::MINOR, Version::REVISION);
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
    SmartPtr<Widget> w = new Widget({ 0.1f, 0.1f, 0.8f, 0.8f }, { 31, 47, 321, 338, 29, 47, 210, 237 });
    SmartPtr<Widget> w1 = new Widget({ 0.3f, 0.3f, 0.8f, 0.8f }, { 31, 47, 321, 338, 29, 47, 210, 237 });
    while (running_) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_QUIT:
                running_ = false;
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
        }

        renderer.StartFrame();
        renderer.SetTexture(Renderer::TEX_TEST);
        renderer.RenderRect({ 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f });
        w->Render();
        w1->Render();
        renderer.RenderText(fpsStr, 0, 0.9f);

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
