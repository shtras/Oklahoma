#pragma once

namespace OGraphics
{
    struct Vertex
    {
        GLfloat x, y, z;
        bool operator== (const Vertex& other) const { return x == other.x && y == other.y && z == other.z; }
    };
    struct UV
    {
        GLfloat u, v;
        bool operator== (const UV& other) const { return u == other.u && v == other.v; }
    };
    struct Rect
    {
        float top, left, width, height;
    };
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
    private:
        GLuint textureID_;
        int width_;
        int height_;
    };

    class Renderer
    {
        enum { POINTS_NUM = 2048 };
    public:
        enum TextureType { TEX_GUI, TEX_FONT };
        enum ShaderType { SHADER_DEFAULT };

        Renderer();
        ~Renderer();

        void Render();
        void Init();
        void DrawRect(Rect&& pos, Rect&& tex);
        void SetTexture(TextureType tex);
        void SetShader(ShaderType shader);
    private:
        void FLush();
        void Destroy();
        void AddVertex(const Vertex&& v, const UV&& uv);
        void Clear();
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

        glm::mat4 mvp_;
        SmartPtr<Shader> shader_;
        SmartPtr<Texture> texture_;
        map<TextureType, SmartPtr<Texture>> textures_;
        map<ShaderType, SmartPtr<Shader>> shaders_;
    };
}
