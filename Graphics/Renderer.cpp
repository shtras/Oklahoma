#include "StdAfx.h"
#include "Renderer.h"

namespace OGraphics
{
#define GLERR {GLenum e; if ((e = glGetError()) != GL_NO_ERROR) {LogError(L"%hs:%d - OpenGL error: %x", __FUNCTION__, __LINE__, e);assert(0);}}
    Shader::Shader() :
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

    const GLuint Shader::GetProgramId() const
    {
        return programId_;
    }

    Texture::Texture():
        textureID_(0)
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
        GLERR
    }

    const GLuint Texture::GetTextureID() const
    {
        return textureID_;
    }

    Renderer::Renderer() :
        mainWindow_(nullptr),
        mainGLContext_(nullptr),
        numVertices_(0),
        numIndexes_(0),
        width_(1280),
        height_(720)
    {

    }

    Renderer::~Renderer()
    {
        Cleanup();
    }

    void Renderer::Prepare()
    {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertices_ * 3, vertices_, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertices_ * 2, uvs_, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndexes_, indexes_, GL_STATIC_DRAW);
    }

    void Renderer::Render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLuint programID = shader_->GetProgramId();
        GLuint textureID = texture_->GetTextureID();
        GLuint matrixLocation = glGetUniformLocation(programID, "MVP");
        GLuint textureLocation = glGetUniformLocation(programID, "sampler");
        glUseProgram(programID);
        glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &mvp_[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(textureLocation, 0);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
        glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer_);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            2,                                // size : U+V => 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
        glDrawElements(
            GL_TRIANGLES,      // mode
            numIndexes_,    // count
            GL_UNSIGNED_SHORT,   // type
            (void*)0           // element array buffer offset
        );
        GLERR
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        
        SDL_GL_SwapWindow(mainWindow_);
        GLERR
    }

    void Renderer::Init()
    {
        Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;

        mainWindow_ = SDL_CreateWindow("SDL2 OpenGL Example", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, width_, height_, flags);
        mainGLContext_ = SDL_GL_CreateContext(mainWindow_);
        glewInit();

        glViewport(0, 0, width_, height_);

        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width_ / (float)height_, 0.1f, 100.0f);
        glm::mat4 View = glm::lookAt(glm::vec3(0, 3, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 Model = glm::mat4(1.0f);
        mvp_ = Projection * View * Model;
        shader_ = new Shader();
        shader_->Load(L"Shaders\\vert.glsl", L"Shaders\\frag.glsl");

        texture_ = new Texture();
        texture_->Load("Textures\\test.png");

        glGenBuffers(1, &vertexBuffer_);
        glGenBuffers(1, &uvBuffer_);
        glGenBuffers(1, &indexBuffer_);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);

        vertices_[numVertices_] = { -1.0f, -1.0f, 0 };
        uvs_[numVertices_++] = { 0.2f, 0.2f };
        vertices_[numVertices_] = { 1.0f, -1.0f, 0 };
        uvs_[numVertices_++] = { 0.8f, 0.2f };
        vertices_[numVertices_] = { -1.0f, 1.0f, 0 };
        uvs_[numVertices_++] = { 0.2f, 0.8f };
        vertices_[numVertices_] = { 1.0f, 1.0f, 0 };
        uvs_[numVertices_++] = { 0.8f, 0.8f };
        indexes_[numIndexes_++] = 0;
        indexes_[numIndexes_++] = 3;
        indexes_[numIndexes_++] = 1;
        indexes_[numIndexes_++] = 0;
        indexes_[numIndexes_++] = 2;
        indexes_[numIndexes_++] = 3;
    }

    void Renderer::Cleanup()
    {
        glDeleteBuffers(1, &vertexBuffer_);
        glDeleteBuffers(1, &uvBuffer_);
        glDeleteBuffers(1, &indexBuffer_);
        SDL_GL_DeleteContext(mainGLContext_);
        SDL_DestroyWindow(mainWindow_);
    }
}
