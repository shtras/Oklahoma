#pragma once
#include "GUIInterface.h"
using namespace OGUI;

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

    class Texture
    {
    public:
        Texture();
        ~Texture();
        void Load(const char* path);
        const GLuint GetTextureID() const;
        const int GetWidth() const;
        const int GetHeight() const;
    private:
        GLuint textureID_;
        int width_;
        int height_;
    };

    class Renderer
    {
        enum { POINTS_NUM = 4096 };
    public:
        enum TextureType { TEX_NONE, TEX_GUI, TEX_FONT, TEX_TEST };
        enum ShaderType { SHADER_DEFAULT };
        static Renderer& GetInstance();
        void StartFrame();
        void RenderFrame();
        void Init();
        void RenderRect(const Rect& pos, const Rect& tex);
        void RenderRect(Rect&& pos, Rect&& tex);
        void RenderText(const wchar_t* text, float x, float y);
        void SetTexture(TextureType tex);
        void SetShader(ShaderType shader);
        void SetCharSize(int width, int height);
        const Texture* GetTexture(TextureType type);
        int GetWidth() const;
        int GetHeight() const;
        bool IsFontSymbol(wchar_t) const;
        void SetBound(const Rect& r);
        void ResetBound();
        void InitUVs(Rect& uvs, TextureType type);
    private:
        Renderer();
        Renderer(const Renderer& other);
        ~Renderer();
        void FLush();
        void Destroy();
        void AddVertex(const Vertex& v, const UV& uv);
        void Clear();
        void LoadTexture(const char* path, TextureType type);
        void LoadShader(const wchar_t* vert, const wchar_t* frag, ShaderType type);
        void InitTextUVs();
        SDL_Window* mainWindow_;
        SDL_GLContext mainGLContext_;
        Vertex vertices_[POINTS_NUM];
        UV uvs_[POINTS_NUM];
        GLushort indexes_[POINTS_NUM];
        GLuint vertexBuffer_;
        GLuint uvBuffer_;
        GLuint indexBuffer_;
        int numVertices_;
        int numIndexes_;
        int width_;
        int height_;
        float charWidth_;
        float charHeight_;
        Rect boundRect_;

        glm::mat4 mvp_;
        SmartPtr<Shader> shader_;
        SmartPtr<Texture> texture_;
        map<TextureType, SmartPtr<Texture>> textures_;
        map<ShaderType, SmartPtr<Shader>> shaders_;
        map<wchar_t, Rect> textUVs_;
        
        int dbgFlushes_;
        int dbgVertices_;
        int dbgRects_;
    };
}
