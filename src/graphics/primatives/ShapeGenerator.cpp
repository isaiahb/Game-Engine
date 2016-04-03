#include "ShapeGenerator.h"
void memcopy(void *dest, void *src, size_t n)
{
    // Typecast src and dest addresses to (char *)
    char *csrc = (char *)src;
    char *cdest = (char *)dest;
    
    // Copy contents of src[] to dest[]
    for (int i=0; i<n; i++)
        cdest[i] = csrc[i];
}

ShapeData ShapeGenerator::makeTriangle() {
    Vertex triangle[] =
    {
        {glm::vec3(+0.0f, +1.0f, +0.0f),    glm::vec3(1.0f, 0.0f, 0.0f)},
        {glm::vec3(-1.0f, -1.0f, +0.0f),    glm::vec3(0.0f, 1.0f, 0.0f)},
        {glm::vec3(+1.0f, -1.0f, +0.0f),    glm::vec3(0.0f, 0.0f, 1.0f)}
    };
    
    GLushort indices[] = {0,1,2};
    
    ShapeData data;
    
    data.numVertices = NUM_ELEMENTS(triangle);
    data.vertices = new Vertex[data.numVertices];
    memcopy(data.vertices, triangle, sizeof(triangle));
    
    data.numIndices = NUM_ELEMENTS(indices);
    data.indices = new GLushort[data.numIndices];
    memcopy(data.indices, indices, sizeof(indices));
    
    return data;
}

ShapeData ShapeGenerator::makeCube(){
    vec3 red(192/255.0f, 57/255.0f, 43/255.0f);
    vec3 up(0,1,0);
    vec3 down(0,-1,0);
    vec3 left(-1,0,0);
    vec3 right(1,0,0);
    vec3 front(0,0,-1);
    vec3 back(0,0,1);
    
    vec2 topLeft(0,0);
    vec2 topRight(1,0);
    vec2 bottomLeft(0,1);
    vec2 bottomRight(1,1);
    
    ShapeData ret;
    Vertex stackVerts[] = {
        {vec3(-1.0f, +1.0f, +1.0f), up, topRight}, // Colour
        {vec3(+1.0f, +1.0f, +1.0f), up, topLeft}, // Colour
        {vec3(+1.0f, +1.0f, -1.0f), up, bottomLeft}, // Colour
        {vec3(-1.0f, +1.0f, -1.0f), up, bottomRight}, // Colour
        //
        
        {vec3(-1.0f, +1.0f, -1.0f), front, topRight}, // Colour
        {vec3(+1.0f, +1.0f, -1.0f), front, topLeft}, // Colour
        {vec3(+1.0f, -1.0f, -1.0f), front, bottomLeft}, // Colour
        {vec3(-1.0f, -1.0f, -1.0f), front, bottomRight}, // Colour
        //
        
        {vec3(+1.0f, +1.0f, -1.0f), right, topRight}, // Colour
        {vec3(+1.0f, +1.0f, +1.0f), right, topLeft}, // Colour
        {vec3(+1.0f, -1.0f, +1.0f), right, bottomLeft}, // Colour
        {vec3(+1.0f, -1.0f, -1.0f), right, bottomRight}, // Colour
        //
        
        {vec3(-1.0f, +1.0f, +1.0f), left, topRight}, // Colour
        {vec3(-1.0f, +1.0f, -1.0f), left, topLeft}, // Colour
        {vec3(-1.0f, -1.0f, -1.0f), left, bottomLeft}, // Colour
        {vec3(-1.0f, -1.0f, +1.0f), left, bottomRight}, // Colour
        //
        
        {vec3(+1.0f, +1.0f, +1.0f), back, topRight}, // Colour
        {vec3(-1.0f, +1.0f, +1.0f), back, topLeft}, // Colour
        {vec3(-1.0f, -1.0f, +1.0f), back, bottomLeft}, // Colour
        {vec3(+1.0f, -1.0f, +1.0f), back, bottomRight}, // Colour
        //
        
        {vec3(+1.0f, -1.0f, -1.0f), down, topRight}, // Colour
        {vec3(-1.0f, -1.0f, -1.0f), down, topLeft}, // Colour
        {vec3(-1.0f, -1.0f, +1.0f), down, bottomLeft}, // Colour
        {vec3(+1.0f, -1.0f, +1.0f), down, bottomRight} // Colour
    };
    
    ret.numVertices = NUM_ELEMENTS(stackVerts);
    ret.vertices = new Vertex[ret.numVertices];
    memcopy(ret.vertices, stackVerts, sizeof(stackVerts));
    
    unsigned short stackIndices[] = {
        0,1,2, 0,2,3,     // Top
        4,  5,  6,  4,  6,  7, // Front
        8,  9, 10,  8, 10, 11, // Right
        12, 13, 14, 12, 14, 15, // Left
        16, 17, 18, 16, 18, 19, // Back
        20, 22, 21, 20, 23, 22 // Bottom
    };
    
    ret.numIndices = NUM_ELEMENTS(stackIndices);
    ret.indices = new GLushort[ret.numIndices];
    memcopy(ret.indices, stackIndices, sizeof(stackIndices));
    
    return ret;
}