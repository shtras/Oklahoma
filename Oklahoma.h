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

    void PerfTest();
    void RegularTest();
private:

    bool running_;
    std::unique_ptr<OGUI::MainWindow> mainWindow_ = nullptr;
};
