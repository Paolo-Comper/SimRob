#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <array>
#include <string>
#include <GLFW/glfw3.h>

class Renderer {
public:
    bool Init();
    bool RenderFrame();
    ~Renderer();

private:
    struct ShaderProgramSource {
        std::string VertexSource;
        std::string FragmentSource;
    };

    GLFWwindow* window;
    unsigned int buffer;
    unsigned int shader;
    int location;

    float r = 0.0f;
    float incremento = 0.05f;

    std::array<float, 8> posizioni = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f};
    std::array<unsigned int, 6> indici = {0, 1, 2, 2, 3, 0};

    void GLClearError();
    bool GLLogCall();
    const char* GlErrorLog(unsigned int error);
    ShaderProgramSource ParseShader(const std::string& filepath);
    GLFWwindow* setupOpenGL(int width, int height, const char* title);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};

#endif
