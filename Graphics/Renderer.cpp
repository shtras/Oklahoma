#include "StdAfx.h"
#include "Renderer.h"
#include "Version.h"

namespace OGraphics
{
#define GLERR {GLenum e; if ((e = glGetError()) != GL_NO_ERROR) {LogError(L"%hs:%d - OpenGL error: %x", __FUNCTION__, __LINE__, e);assert(0);}}
    Shader::Shader() noexcept :
        programId_(0)
    {

    }

    Shader::~Shader()
    {
    }

    void Shader::Load(const wchar_t* vert, const wchar_t* frag)
    {
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        std::string VertexShaderCode;
        std::ifstream VertexShaderStream(vert, std::ios::in);
        if (VertexShaderStream.is_open()) {
            std::string Line = "";
            while (getline(VertexShaderStream, Line))
                VertexShaderCode += "\n" + Line;

            VertexShaderStream.close();
        } else {
            LogError(L"Could not open vertex shader: %ls", vert);
            return;
        }

        // Read the Fragment Shader code from the file
        std::string FragmentShaderCode;
        std::ifstream FragmentShaderStream(frag, std::ios::in);
        if (FragmentShaderStream.is_open()) {
            std::string Line = "";
            while (getline(FragmentShaderStream, Line))
                FragmentShaderCode += "\n" + Line;

            FragmentShaderStream.close();
        } else {
            LogError(L"Could not open fragment shader: %ls", frag);
            return;
        }

        GLint Result = GL_FALSE;
        int InfoLogLength;

        // Compiling shader
        char const * VertexSourcePointer = VertexShaderCode.c_str();
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
        glCompileShader(VertexShaderID);

        // Check Vertex Shader
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            LogError(L"Error in vertex shader: %hs", &VertexShaderErrorMessage[0]);
        }

        // Compile Fragment Shader
        char const * FragmentSourcePointer = FragmentShaderCode.c_str();
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
        glCompileShader(FragmentShaderID);

        // Check Fragment Shader
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            LogError(L" Error in fragment shader: %hs", &FragmentShaderErrorMessage[0]);
        }

        // Link the program
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);

        // Check the program
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            LogError(L"Error in shader link: %hs", &ProgramErrorMessage[0]);
        }

        glDetachShader(ProgramID, VertexShaderID);
        glDetachShader(ProgramID, FragmentShaderID);

        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);

        programId_ = ProgramID;
    }

    const GLuint Shader::GetProgramID() const
    {
        return programId_;
    }

    Texture::Texture() noexcept:
        textureID_(0),
        width_(-1),
        height_(-1)
    {

    }

    Texture::~Texture()
    {
        if (textureID_ > 0) {
            glDeleteTextures(1, &textureID_);
        }
    }

    void Texture::Load(const char* path)
    {
        SDL_Surface* fontSurf = IMG_Load(path);
        if (!fontSurf) {
            LogError(L"Failed to load file: %hs", path);
            return;
        }
        glGenTextures(1, &textureID_);
        glBindTexture(GL_TEXTURE_2D, textureID_);
        int mode = GL_RGB;
        if (fontSurf->format->BytesPerPixel == 4) {
            mode = GL_RGBA;
        }
        width_ = fontSurf->w;
        height_ = fontSurf->h;
        glTexImage2D(GL_TEXTURE_2D, 0, mode, fontSurf->w, fontSurf->h, 0, mode, GL_UNSIGNED_BYTE, fontSurf->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GLERR;
    }

    const GLuint Texture::GetTextureID() const
    {
        return textureID_;
    }

    const int Texture::GetWidth() const
    {
        return width_;
    }

    const int Texture::GetHeight() const
    {
        return height_;
    }

    Renderer::Renderer() noexcept :
        mainWindow_(nullptr),
        mainGLContext_(nullptr),
        numVertices_(0),
        numIndexes_(0),
        width_(1280),
        height_(720),
        dbgFlushes_(0),
        dbgVertices_(0),
        dbgRects_(0),
        mouseCursor_(MouseCursor::Default),
        mousePos_{0, 0, 0, 0}
    {

    }

    Renderer::~Renderer()
    {
        destroy();
    }

    Renderer& Renderer::GetInstance()
    {
        static Renderer instance;
        return instance;
    }

    void Renderer::StartFrame()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::RenderFrame()
    {
        renderMouseCursor();
        if (numVertices_ > 0) {
            fLush();
        }

        //wcout << L"F: " << dbgFlushes_ << L" V: " << dbgVertices_ << L" R: " << dbgRects_ << endl;
        dbgFlushes_ = 0;
        dbgVertices_ = 0;
        dbgRects_ = 0;

        SDL_GL_SwapWindow(mainWindow_);
        GLERR;
        clear();
    }

    void Renderer::Init()
    {
        SDL_Init(SDL_INIT_EVERYTHING);
        IMG_Init(IMG_INIT_PNG);
        Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;

        char title[128];
        sprintf_s(title, "Oklahoma %d.%d.%d", Version::MAJOR, Version::MINOR, Version::REVISION);

        mainWindow_ = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_, height_, flags);
        mainGLContext_ = SDL_GL_CreateContext(mainWindow_);
        glewInit();

        glViewport(0, 0, width_, height_);
        
        glm::mat4 Projection = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f, -5.0f, 5.0f);
        glm::mat4 View = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 Model = glm::mat4(1.0f);
        mvp_ = Projection;
        loadShader(L"Shaders\\vert.glsl", L"Shaders\\frag.glsl", ShaderType::Default);
        SetShader(ShaderType::Default);

        loadTexture("Textures\\gui0.png", TextureType::GUI);
        loadTexture("Textures\\Font.png", TextureType::Font);
        loadTexture("Textures\\test.png", TextureType::Test);
        initTextUVs();
        initMouseCursorUVs();

        SetCharSize(15, 20);
        mousePos_.width = 32.0f / (float)width_;
        mousePos_.height = 32.0f / (float)height_;

        glGenBuffers(1, &vertexBuffer_);
        glGenBuffers(1, &uvBuffer_);
        glGenBuffers(1, &indexBuffer_);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0);
        boundRectQueue_.push_back({ -1.0f, -1.0f, 2.0f, 2.0f });
        SDL_ShowCursor(0);
    }

    void Renderer::fLush()
    {
        if (numIndexes_ == 0) {
            return;
        }
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertices_ * 3, vertices_, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertices_ * 2, uvs_, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndexes_, indexes_, GL_STATIC_DRAW);

        GLuint programID = shader_->GetProgramID();
        GLuint textureID = texture_->GetTextureID();
        GLuint matrixLocation = glGetUniformLocation(programID, "MVP");
        GLuint textureLocation = glGetUniformLocation(programID, "sampler");
        GLuint boundLocation = glGetUniformLocation(programID, "bound");
        glUseProgram(programID);
        glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &mvp_[0][0]);
        Rect& boundRect = boundRectQueue_.back();
        glUniform4f(boundLocation, boundRect.left * 2.0f - 1.0f, (boundRect.top + boundRect.height) * -2.0f + 1.0f, (boundRect.left + boundRect.width) * 2.0f - 1.0f, boundRect.top * -2.0f + 1.0f);

        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(textureLocation, 0);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer_);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
        glDrawElements(GL_TRIANGLES, numIndexes_, GL_UNSIGNED_SHORT, NULL);
        GLERR;
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        numVertices_ = 0;
        numIndexes_ = 0;
        ++dbgFlushes_;
    }

    void Renderer::RenderRect(const Rect& pos, const Rect& tex)
    {
        if (numIndexes_ >= POINTS_NUM - 6) {
            fLush();
        }
        
        addVertex({ pos.left, pos.top, 0 }, { tex.left, tex.top });
        addVertex({ pos.left + pos.width, pos.top + pos.height, 0 }, { tex.left + tex.width, tex.top + tex.height });
        addVertex({ pos.left + pos.width, pos.top, 0 }, { tex.left + tex.width, tex.top });
        addVertex(-3);
        addVertex({ pos.left, pos.top + pos.height, 0 }, { tex.left, tex.top + tex.height });
        addVertex(-3);
        ++dbgRects_;
    }

    void Renderer::RenderRect(Rect&& pos, Rect&& tex)
    {
        RenderRect(pos, tex);
    }

    void Renderer::addVertex(const Vertex& v, const UV& uv)
    {
        if (!texture_) {
            LogError(L"No active texture");
            assert(0);
        }
        vertices_[numVertices_] = v;
        uvs_[numVertices_++] = uv;
        indexes_[numIndexes_++] = numVertices_ - 1;
        ++dbgVertices_;
    }

    void Renderer::addVertex(int didx)
    {
        indexes_[numIndexes_++] = numVertices_ + didx;
    }

    void Renderer::RenderText(const wchar_t* text, float x, float y)
    {
        SetTexture(TextureType::Font);
        size_t len = wcslen(text);
        for (int i = 0; i < len; ++i) {
            wchar_t c = text[i];
            if (textUVs_.count(c) == 0) {
                c = ' ';
            }
            Rect& uv = textUVs_[c];
            RenderRect({ x + i*charWidth_, y, charWidth_, charHeight_ }, uv);
        }
    }

    void Renderer::SetTexture(TextureType tex)
    {
        if (texture_ == textures_[tex]) {
            return;
        }
        fLush();
        texture_ = textures_[tex];
    }

    void Renderer::SetShader(ShaderType shader)
    {
        if (shader_ == shaders_[shader]) {
            return;
        }
        fLush();
        shader_ = shaders_[shader];
    }

    void Renderer::SetCharSize(int width, int height)
    {
        charWidthLast_ = charWidth_;
        charHeightLast_ = charHeight_;
        charWidth_ = width / (float)width_;
        charHeight_ = height / (float)height_;
    }

    void Renderer::SetMouseCursor(MouseCursor cursor)
    {
        mouseCursor_ = cursor;
    }

    void Renderer::SetMouseCoords(float x, float y)
    {
        mousePos_.left = x - mousePos_.width * 0.5f;
        mousePos_.top = y - mousePos_.height * 0.5f;
    }

    void Renderer::ResetCharSize()
    {
        charWidth_ = charWidthLast_;
        charHeight_ = charHeightLast_;
    }

    std::shared_ptr<Texture> Renderer::GetTexture(TextureType type)
    {
        return textures_[type];
    }

    int Renderer::GetWidth() const
    {
        return width_;
    }

    int Renderer::GetHeight() const
    {
        return height_;
    }

    bool Renderer::IsFontSymbol(wchar_t c) const
    {
        return textUVs_.count(c) > 0;
    }

    void Renderer::PushBound(const Rect& r)
    {
        if (r != boundRectQueue_.back()) {
            fLush();
        }
        Rect& newR = boundRectQueue_.back().Intersect(r);
        boundRectQueue_.push_back(newR);
    }

    void Renderer::PopBound()
    {
        Rect r = boundRectQueue_.back();
        fLush();
        boundRectQueue_.pop_back();
    }

    void Renderer::InitUVs(Rect& uvs, TextureType type)
    {
        auto tex = GetTexture(type);
        uvs.left /= (float)tex->GetWidth();
        uvs.width /= (float)tex->GetWidth();
        uvs.top /= (float)tex->GetHeight();
        uvs.height /= (float)tex->GetHeight();
    }

    float Renderer::GetPixelWidth() const
    {
        return 1 / (float)width_;
    }

    float Renderer::GetPixelHeight() const
    {
        return 1 / (float)height_;
    }

    void Renderer::destroy()
    {
        glDeleteBuffers(1, &vertexBuffer_);
        glDeleteBuffers(1, &uvBuffer_);
        glDeleteBuffers(1, &indexBuffer_);
        SDL_GL_DeleteContext(mainGLContext_);
        SDL_DestroyWindow(mainWindow_);
    }

    void Renderer::clear()
    {
        numVertices_ = 0;
        numIndexes_ = 0;
    }

    void Renderer::loadTexture(const char* path, TextureType type)
    {
        auto texture = std::make_shared<Texture>();
        texture->Load(path);
        textures_[type] = texture;
    }

    void Renderer::loadShader(const wchar_t* vert, const wchar_t* frag, ShaderType type)
    {
        auto shader = std::make_shared<Shader>();
        shader->Load(vert, frag);
        shaders_[type] = shader;
    }

    void Renderer::initTextUVs()
    {
        int charWidth = 19;
        int charHeight = 41;
        int texWidth = textures_[TextureType::Font]->GetWidth();
        int texHeight = textures_[TextureType::Font]->GetHeight();
        float fCharWidth = charWidth / (float)texWidth;
        float fCharHeight = charHeight / (float)texHeight;
        std::wifstream f(L"Textures\\font.txt", std::ios::in);
        if (!f.is_open()) {
            LogError(L"Failed to open characters file");
            return;
        }
        std::vector<std::wstring> fontLines;
        std::wstring s;
        while (getline(f, s)) {
            fontLines.push_back(s);
        }
        f.close();
        for (int i = 0; i < fontLines.size(); ++i) {
            std::wstring& sitr = fontLines[i];
            for (int x = 0; x <= sitr.size(); ++x) {
                textUVs_[sitr[x]] = { x * fCharWidth, i * fCharHeight, fCharWidth, fCharHeight };
            }
        }
    }

    void Renderer::initMouseCursorUVs()
    {
        cursorUVs_[MouseCursor::Default] = Rect{ 234,2,34,34 };
        cursorUVs_[MouseCursor::ResizeHorizontal] = Rect{ 266,2,32,32 };
        cursorUVs_[MouseCursor::ResizeVertical] = Rect{ 295,2,32,32 };
        cursorUVs_[MouseCursor::ResizeDiagonalNE] = Rect{ 294,30,32,32 };
        cursorUVs_[MouseCursor::ResizeDiagonalNW] = Rect{ 264,27,32,32 };

        float texWidth = (float)textures_[TextureType::GUI]->GetWidth();
        float texHeight = (float)textures_[TextureType::GUI]->GetHeight();
        for (auto& itr : cursorUVs_) {
            itr.second.left /= texWidth;
            itr.second.top /= texHeight;
            itr.second.width /= texWidth;
            itr.second.height /= texHeight;
        }
    }

    void Renderer::renderMouseCursor()
    {
        SetTexture(TextureType::GUI);
        RenderRect(mousePos_, cursorUVs_[mouseCursor_]);
    }

}
