#include "StdAfx.h"
#include "Renderer.h"

namespace OGraphics
{

    Renderer::Renderer():
        mainWindow_(nullptr),
        mainGLContext_(nullptr)
    {

    }

    Renderer::~Renderer()
    {
    }

    void Renderer::Render()
    {
        SDL_GL_SwapWindow(mainWindow_);
    }

    void Renderer::Init()
    {
        Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
        int width = 1024;
        int height = 768;

        mainWindow_ = SDL_CreateWindow("SDL2 OpenGL Example", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, width, height, flags);
        mainGLContext_ = SDL_GL_CreateContext(mainWindow_);
        glewInit();
    }

}
