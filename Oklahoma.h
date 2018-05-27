#pragma once
#include "Renderer.h"

class Oklahoma
{
public:
    static Oklahoma& GetInstance();
    void Run();
    void Init();
    void Quit();
private:
    Oklahoma() noexcept;
    ~Oklahoma();

    bool running_;
};
