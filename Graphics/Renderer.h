#pragma once
#include "GUIInterface.h"
using namespace OGUI;

namespace OGraphics
{
    class Shader
    {
    public:
        Shader() noexcept;
        ~Shader();

        void Load(const wchar_t* vert, const wchar_t* frag);
        const GLuint GetProgramID() const;
    private:
        GLuint programId_;
    };

    class Texture
    {
    public:
        Texture() noexcept;
        Texture(Texture& other) = delete;
        Texture(Texture&& other) = delete;
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
        enum class TextureType { None, GUI, Font, Test };
        enum class ShaderType { Default };
        enum class MouseCursor { Default, ResizeHorizontal, ResizeVertical, ResizeDiagonalNE, ResizeDiagonalNW };
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
        void SetMouseCursor(MouseCursor cursor);
        void SetMouseCoords(float x, float y);
        void ResetCharSize();
        std::shared_ptr<Texture> GetTexture(TextureType type);
        int GetWidth() const;
        int GetHeight() const;
        bool IsFontSymbol(wchar_t) const;
        void PushBound(const Rect& r);
        void PopBound();
        void InitUVs(Rect& uvs, TextureType type);
        float GetPixelWidth() const;
        float GetPixelHeight() const;
    private:
        Renderer() noexcept;
        Renderer(const Renderer& other) = delete;
        ~Renderer();
        void fLush();
        void destroy();
        void addVertex(const Vertex& v, const UV& uv);
        void addVertex(int didx);
        void clear();
        void loadTexture(const char* path, TextureType type);
        void loadShader(const wchar_t* vert, const wchar_t* frag, ShaderType type);
        void initTextUVs();
        void initMouseCursorUVs();
        void renderMouseCursor();
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
        float charWidthLast_;
        float charHeightLast_;
        std::list<Rect> boundRectQueue_;

        glm::mat4 mvp_;
        std::shared_ptr<Shader> shader_;
        std::shared_ptr<Texture> texture_;
        MouseCursor mouseCursor_;
        Rect mousePos_;
        std::map<TextureType, std::shared_ptr<Texture>> textures_;
        std::map<ShaderType, std::shared_ptr<Shader>> shaders_;
        std::map<wchar_t, Rect> textUVs_;
        std::map<MouseCursor, Rect> cursorUVs_;
        
        int dbgFlushes_;
        int dbgVertices_;
        int dbgRects_;
    };
}
