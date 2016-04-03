#pragma once
#include <GL/glew.h>
#include "Vertex.h"

struct ShapeData {
    ShapeData() : vertices(0), numVertices(0), indices(0), numIndices(0){}
    
    Vertex *vertices;
    GLuint numVertices;
    
    GLushort *indices;
    GLuint numIndices;
    
    GLsizeiptr vertexBufferSize() const {
        return (numVertices+1) * sizeof(Vertex);
    }
    
    GLsizeiptr indexBufferSize() const {
        return (numIndices+1) * sizeof(GLushort);
    }
    
    void cleanup() {
        delete [] vertices;
        delete [] indices;
        numIndices = numIndices = NULL;
    }
};
