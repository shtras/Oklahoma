#pragma once
#include "Renderer.h"

class Oklahoma
{
public:
    static Oklahoma& GetInstance();
    void Run();
    void Init();
private:
    Oklahoma();
    ~Oklahoma();

    SmartPtr<OGraphics::Renderer> renderer_;
    bool running_;
};
