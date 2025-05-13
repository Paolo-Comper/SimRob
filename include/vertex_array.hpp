#pragma once

#include "renderer.hpp"
#include "vertex_buffer.hpp"
#include "vertex_buffer_layout.hpp"

class VertexArray {

    private:
        unsigned int m_renderer_id;
        

    public:

        VertexArray();
        ~VertexArray();

        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

        void Bind () const;
        void Unbind () const;

        
};

