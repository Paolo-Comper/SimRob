#include "index_buffer.hpp"

#include "renderer.hpp"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_count(count) {
    GLCall(glGenBuffers(1, &m_renderer_id));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() { GLCall(glDeleteBuffers(1, &m_renderer_id)); }

void IndexBuffer::Bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id)); }

void IndexBuffer::Unbind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }