#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <string>
#include <GLFW/glfw3.h>
#include "settings.hpp"

void GLClearError();
bool GLLogCall(int row, const char* file);
const char* GlErrorLog(unsigned int error);

#if DEBUG
    #define GLCall(x) \
        GLClearError();\
        x; \
        GLLogCall(__LINE__,__FILE__);
#else
    #define GLCall(x)\
        x;
#endif

#endif
