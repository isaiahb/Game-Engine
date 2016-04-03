#pragma once
#include <vector>
#include <GL/glew.h>
#include "Buffer.hpp"

namespace ballah { namespace graphics {
    class VertexArray {
    private:
        GLuint arrayID;
        std::vector<Buffer*> buffers;
    public:
        VertexArray();
        ~VertexArray();
        void addBuffer(Buffer *buffer, GLuint index);
        void bind() const;
        void unbind() const;
    };
}}