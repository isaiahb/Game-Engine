//
//  IndexBuffer.cpp
//  OpenGL
//
//  Created by isaiah ballah on 2016-03-19.
//  Copyright © 2016 isaiah ballah. All rights reserved.
//

#include "IndexBuffer.hpp"

namespace ballah { namespace graphics {
    IndexBuffer::IndexBuffer(GLushort *data, GLsizei count) {
        this->count = count;
        
        glGenBuffers(1, &bufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLushort), data, GL_STATIC_DRAW);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    void IndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
    }
    void IndexBuffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
    }
}}
