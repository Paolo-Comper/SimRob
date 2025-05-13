#include "logger.hpp"
#include "renderer.hpp"

#include "index_buffer.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"

#include <fstream>
#include <sstream>

#include <iostream>

// Struttura dati che contiene le due shader

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

// Esamina il file .shader e cerca il tag (#shader) se lo trova cerca al suo interno i 2 tag che
// definiscono i 2 tipi di shader (fragment e vertex) successivamente le salva separatamente nella
// struct creata in precedenza
//! Il programma necessita obbligatoriamente di entrambe le shader

ShaderProgramSource ParseShader(const std::string& filepath) {
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

// Viene creato una finestra (contesto valido per OpenGL) da GLFW

GLFWwindow* setupOpenGL(int width, int height, const char* title) {

    // Inizializza GLFW
    if (!glfwInit()) {
        spdlog::error("RENDERER: Error inizializing GLFW");
        return nullptr;
    }

    // Specifica la versione di OpenGL in uso
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Crea la finestra
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        spdlog::error("RENDERER: Error during the window making process");
        glfwTerminate();
        return nullptr;
    }

    // Specifica che il contesto di rendering di OpenGL è la finestra appena creata
    glfwMakeContextCurrent(window);

    // Controlla che OpenGL sia stato inizializzato con succcesso
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        spdlog::error("RENDERER: Error inizializing GLAD");
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    return window;
}

// Compila le shader e le passa a OpenGL

unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type); // Ritorna l'id delle shader create
    const char* src = source.c_str();
    GLCall(glShaderSource(id,           // Specifica da dove prendere la shader,
                          1,            // 1 è quante shader ci sono,
                          &src,         // src deve essre una reference a un const char*, 
                          nullptr));    //nullptr significa che deve trovare lui la lunghezza della shader

                         
    GLCall(glCompileShader(id)); // Compila le shader

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length);
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        spdlog::error("RENDERER: Faild to compile {} shader",
                      (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
        spdlog::error("RENDERER: {}", message);
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
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

GLFWwindow* window;

ShaderProgramSource source;

unsigned int shader;
int location;

float r = 0.0f;
float incremento = 0.05f;

std::array<float, 8> positions = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f};
std::array<unsigned int, 6> indices = {0, 1, 2, 2, 3, 0};

int main() {

    window = setupOpenGL(1080, 720, "Window");

    glfwSwapInterval(1);
    {
        VertexArray va;
        VertexBuffer vb(positions.data(), sizeof(positions));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.Bind();
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices.data(),
                       6); // TODO: 6 da modificare e mettere la shape dell'array (quanti elementi)

        ib.Bind();

        ShaderProgramSource source = ParseShader("resources/shaders/Basic.shader");
        shader = CreateShader(source.VertexSource, source.FragmentSource);

        GLCall(glUseProgram(shader));

        location = glGetUniformLocation(shader, "u_color");
        assert(location != -1);
        GLCall(glUniform3f(location, 1.0f, 0.0f, 0.0f));

        while (!glfwWindowShouldClose(window)) {

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
        }
    }
    glfwTerminate();
}
