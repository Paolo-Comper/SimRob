#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer.hpp"
#include "logger.hpp"
#include <fstream>
#include <sstream>
#include <cassert>
#include "utils.hpp"

void Renderer::GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool Renderer::GLLogCall() {
    while (GLenum error = glGetError()) {
        spdlog::error("RENDERER: {}", GlErrorLog(error));
        return false;
    }
    return true;
}

const char* Renderer::GlErrorLog(unsigned int error) {
    switch (error) {
        case GL_NO_ERROR: return "GL_NO_ERROR: No error has been recorded.";
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE: A numeric argument is out of range.";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
        default: return "Unknown OpenGL error.";
    }
}

Renderer::ShaderProgramSource Renderer::ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
    std::stringstream ss[2];
    std::string line;
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        } else {
            ss[static_cast<int>(type)] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}

GLFWwindow* Renderer::setupOpenGL(int width, int height, const char* title) {
    if (!glfwInit()) {
        spdlog::error("RENDERER: Error inizializing GLFW");
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        spdlog::error("RENDERER: Error during the window making process");
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        spdlog::error("RENDERER: Error inizializing GLAD");
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    return window;
}

unsigned int Renderer::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length);
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        spdlog::error("RENDERER: Faild to compile {} shader", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
        spdlog::error("RENDERER: {}", message);
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Renderer::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    if (vs == 0 || fs == 0) {
        spdlog::error("RENDERER: Shader compilation failed");
        return 0;
    }


    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

bool Renderer::Init() {
    window = setupOpenGL(1080, 720, "Window");

    glfwSwapInterval(1);

    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(posizioni), posizioni.data(), GL_STATIC_DRAW));

    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr));

    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indici), indici.data(), GL_STATIC_DRAW));

    ShaderProgramSource source = ParseShader("resources/shaders/Basic.shader");
    shader = CreateShader(source.VertexSource, source.FragmentSource);

    GLCall(glUseProgram(shader));

    location = glGetUniformLocation(shader, "u_color");
    assert(location != -1);
    GLCall(glUniform3f(location, 1.0f, 0.0f, 0.0f));

    return true;
}

bool Renderer::RenderFrame() {
    if (glfwWindowShouldClose(window)) return false;

    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    GLCall(glUniform3f(location, 0.5f, 1.0f - r, r));
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    if (r > 1.0f)
        incremento = -0.05f;
    else if (r < 0.0f)
        incremento = 0.05f;

    r += incremento;

    glfwSwapBuffers(window);
    glfwPollEvents();

    return true;
}


Renderer::~Renderer() {
    glfwTerminate();
}
