#pragma once
#include "Renderer.h"

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
};
