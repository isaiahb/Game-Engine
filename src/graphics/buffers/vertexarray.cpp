//
//  VertexArray.cpp
//  OpenGL
//
//  Created by isaiah ballah on 2016-03-19.
//  Copyright © 2016 isaiah ballah. All rights reserved.
//

#include "VertexArray.hpp"
#include <iostream>

namespace ballah { namespace graphics {
    VertexArray::VertexArray(){
        glGenVertexArrays(1, &arrayID);
    }
    VertexArray::~VertexArray(){
        for (int i = 0; i < buffers.size(); i++)
            delete buffers[i];
    }
    
    void VertexArray::addBuffer(Buffer *buffer, GLuint index){
        bind();
        buffer->bind();
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, buffer->getComponentCount() , GL_FLOAT, GL_FALSE, 0, 0);
        //glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * buffer->getComponentCount(), 0);
        
        buffer->unbind();
        unbind();
    }
    void VertexArray::bind() const{
        glBindVertexArray(arrayID);
    }
    void VertexArray::unbind() const{
        glBindVertexArray(0);
    }
}}