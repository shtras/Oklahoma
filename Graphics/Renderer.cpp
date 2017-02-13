#include "StdAfx.h"
#include "Renderer.h"

namespace OGraphics
{
#define GLERR {GLenum e; if ((e = glGetError()) != GL_NO_ERROR) {LogError(L"%hs:%d - OpenGL error: %d", __FUNCTION__, __LINE__, e);assert(0);}}
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
            LogError(L"Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vert);
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

    Renderer::Renderer() :
        mainWindow_(nullptr),
        mainGLContext_(nullptr),
        numVertices_(0),
        numIndexes_(0),
        numColors_(0),
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
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertices_, vertices_, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numColors_, colors_, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndexes_, indexes_, GL_STATIC_DRAW);
    }
    void Renderer::Render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLuint matrixID = glGetUniformLocation(shader_->GetProgramId(), "MVP");
        glUseProgram(shader_->GetProgramId());
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp_[0][0]);

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
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer_);
        glVertexAttribPointer(
            1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
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
    }

    void Renderer::Init()
    {
        Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;

        mainWindow_ = SDL_CreateWindow("SDL2 OpenGL Example", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, width_, height_, flags);
        mainGLContext_ = SDL_GL_CreateContext(mainWindow_);
        glewInit();

        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width_ / (float)height_, 0.1f, 100.0f);
        glm::mat4 View = glm::lookAt(glm::vec3(0, 3, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 Model = glm::mat4(1.0f);
        mvp_ = Projection * View * Model;
        shader_ = new Shader();
        shader_->Load(L"Shaders\\vert.glsl", L"Shaders\frag.glsl");

        glGenBuffers(1, &vertexBuffer_);
        glGenBuffers(1, &colorBuffer_);
        glGenBuffers(1, &indexBuffer_);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);

        vertices_[numVertices_++] = -1.0f;
        vertices_[numVertices_++] = -1.0f;
        vertices_[numVertices_++] = 0;
        vertices_[numVertices_++] = 1.0f;
        vertices_[numVertices_++] = -1.0f;
        vertices_[numVertices_++] = 0;
        vertices_[numVertices_++] = -1.0f;
        vertices_[numVertices_++] = 1.0f;
        vertices_[numVertices_++] = 0;
        vertices_[numVertices_++] = 1.0f;
        vertices_[numVertices_++] = 1.0f;
        vertices_[numVertices_++] = 0;

        colors_[numColors_++] = 1.0f;
        colors_[numColors_++] = 1.0f;
        colors_[numColors_++] = 0;
        colors_[numColors_++] = 1.0f;
        colors_[numColors_++] = 1.0f;
        colors_[numColors_++] = 0;
        colors_[numColors_++] = 1.0f;
        colors_[numColors_++] = 1.0f;
        colors_[numColors_++] = 0;
        colors_[numColors_++] = 1.0f;
        colors_[numColors_++] = 1.0f;
        colors_[numColors_++] = 0;

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
        glDeleteBuffers(1, &colorBuffer_);
        glDeleteBuffers(1, &indexBuffer_);
        SDL_GL_DeleteContext(mainGLContext_);
        SDL_DestroyWindow(mainWindow_);
    }

}
