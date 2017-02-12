#pragma once

namespace OGraphics
{
    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        void Render();
        void Init();
    private:
        SDL_Window* mainWindow_;
        SDL_GLContext mainGLContext_;
    };
}
