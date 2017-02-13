#pragma once

namespace OGraphics
{
    class Shader
    {
    public:
        Shader();
        ~Shader();

        void Load(const wchar_t* vert, const wchar_t* frag);
        const GLuint GetProgramId() const;
    private:
        GLuint programId_;
    };

    class Renderer
    {
        enum { POINTS_NUM = 2048 };
    public:
        Renderer();
        ~Renderer();

        void Render();
        void Init();
    private:
        SDL_Window* mainWindow_;
        SDL_GLContext mainGLContext_;
        GLfloat vertices_[POINTS_NUM * 3];
        GLfloat indexes_[POINTS_NUM];
        int numVertices_;
        int numIndexes_;
        int width_;
        int height_;

        glm::mat4 mvp_;
        SmartPtr<Shader> shader_;
    };
}
