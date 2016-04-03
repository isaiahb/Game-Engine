//
//  Buffer.cpp
//  OpenGL
//
//  Created by isaiah ballah on 2016-03-19.
//  Copyright © 2016 isaiah ballah. All rights reserved.
//

#include "Buffer.hpp"
#include <iostream>
namespace ballah { namespace graphics {
    Buffer::Buffer(GLfloat *data, GLsizei count, GLuint componentCount) {
        this->componentCount = componentCount;
        std::cout<<"count "<<count<<std::endl;
        glGenBuffers(1, &bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), data, GL_STATIC_DRAW);
        
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    void Buffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    }
    void Buffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }
}}

//
