#include "StdAfx.h"
#include "Oklahoma.h"
#include "Version.h"

using namespace OGraphics;

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
        renderer_->StartFrame();

        //renderer_->SetTexture(Renderer::TEX_GUI);
        //renderer_->RenderRect(Rect({ 0.0f, 0.0f, 0.5f, 0.5f }), Rect({ 0.0f, 0.0f, 1.0f, 1.0f }));
        //renderer_->RenderRect(Rect({ 0.5f, 0.0f, 0.3f, 0.3f }), Rect({ 0.0f, 0.0f, 0.5f, 0.5f }));

        renderer_->RenderText(L"abcdefghijklmnopqrstuvwxyz", 0, 0);
        renderer_->RenderText(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0, 0.3f);
        renderer_->RenderText(L"0123456789/", 0, 0.5f);
        renderer_->RenderText(L"!@#$%^&*()-=_+,.;:'\"[]{}\\|", 0, 0.7f);

        renderer_->RenderFrame();
    }
}

void Oklahoma::Init()
{
    renderer_ = new Renderer();
    renderer_->Init();
}

Oklahoma::Oklahoma() :
    running_(true)
{

}

Oklahoma::~Oklahoma()
{
}
