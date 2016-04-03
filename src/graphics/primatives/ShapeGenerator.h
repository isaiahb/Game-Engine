#pragma once
#include <stdio.h>
#include "ShapeData.h"
#include <glm/glm.hpp>
#include "Vertex.h"

#define NUM_ELEMENTS(array) sizeof(array) / sizeof(*array)
using namespace glm;

class ShapeGenerator {
public:
    static ShapeData makeTriangle();
    static ShapeData makeCube();
};
