#pragma once

#include <renderer.hpp>
#include <vector>
#include <cassert>

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT:         return 4;
            case GL_UNSIGNED_INT:  return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        assert(false); // Tipo non supportato
        return 0;
    }
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferElement> m_elements;
    unsigned int m_stride;

public:
    VertexBufferLayout()
        : m_stride(0) {}

    template<typename T>
    void Push(unsigned int count); // Generico: dichiarazione sola

    inline const std::vector<VertexBufferElement>& GetElements() const { return m_elements; }
    inline unsigned int GetStride() const { return m_stride; }
};
