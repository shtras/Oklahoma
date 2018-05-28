#pragma once
#include "Renderer.h"
#include "MainWindow.h"

class Oklahoma
{
public:
    Oklahoma() noexcept;
    ~Oklahoma();

    void Run();
    void Init();
    void Quit();

private:
    void perfTest();
    void regularTest();

    bool running_;
    std::unique_ptr<OGUI::MainWindow> mainWindow_ = nullptr;
};
