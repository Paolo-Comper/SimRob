#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <string>
#include <GLFW/glfw3.h>
#include "utils.hpp"

void GLClearError();
bool GLLogCall();
const char* GlErrorLog(unsigned int error);

#ifdef DEBUG
    #define GLCall(x) \
        GLClearError();\
        x; \
        GLLogCall();
#else
    #define GLCall(x)\
        x;
#endif

#endif
