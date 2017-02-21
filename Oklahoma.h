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
    Oklahoma();
    ~Oklahoma();

    bool running_;
};
