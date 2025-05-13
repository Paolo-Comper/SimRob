
#include "renderer.hpp"
#include "logger.hpp"

#include "settings.hpp"
#include <cassert>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool GLLogCall(int row, const char* file) {
    while (GLenum error = glGetError()) {
        spdlog::error("RENDERER: {} row: {}, file: {}", GlErrorLog(error), row, file);
        return false;
    }
    return true;
}

const char* GlErrorLog(unsigned int error) {
    switch (error) {
        case GL_NO_ERROR:
            return "GL_NO_ERROR: No error has been recorded.";
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated "
                   "argument.";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE: A numeric argument is out of range.";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION: The specified operation is not allowed in the current "
                   "state.";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
        default:
            return "Unknown OpenGL error.";
    }
}