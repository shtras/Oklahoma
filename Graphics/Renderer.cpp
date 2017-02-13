#include "StdAfx.h"
#include "Renderer.h"

namespace OGraphics
{

    Shader::Shader():
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
        width_(1280),
        height_(720)
    {

    }

    Renderer::~Renderer()
    {
    }

    void Renderer::Render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLuint matrixID = glGetUniformLocation(shader_->GetProgramId(), "MVP");
        glUseProgram(shader_->GetProgramId());
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp_[0][0]);



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
    }
}
