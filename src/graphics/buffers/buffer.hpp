#pragma once
#include <GL/glew.h>

namespace ballah { namespace graphics {
    
    class Buffer {
    private:
        GLuint bufferID;
        GLuint componentCount;
        
    public:
        Buffer(GLfloat *data, GLsizei count, GLuint componentCount);
        
        void bind() const;
        void unbind() const;
        inline GLuint getComponentCount() const {return componentCount;}
    };
}}