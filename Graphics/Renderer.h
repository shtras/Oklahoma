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

        void Prepare();
        void Render();
        void Init();
    private:
        void Cleanup();
        SDL_Window* mainWindow_;
        SDL_GLContext mainGLContext_;
        GLfloat vertices_[POINTS_NUM * 3];
        GLfloat colors_[POINTS_NUM * 3];
        GLushort indexes_[POINTS_NUM];
        GLuint vertexBuffer_;
        GLuint colorBuffer_;
        GLuint indexBuffer_;
        int numVertices_;
        int numColors_;
        int numIndexes_;
        int width_;
        int height_;

        glm::mat4 mvp_;
        SmartPtr<Shader> shader_;
    };
}
