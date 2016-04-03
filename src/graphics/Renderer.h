#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "Block.h"
#include "ShapeGenerator.h"
#include "SOIL.h"
#include "Shader.hpp"

using namespace std;
using  namespace ballah;
using namespace graphics;

class Renderer {
private:
    ShapeData blockShape;
    GLuint blockVAO;
    GLuint skyboxVAO;
    Shader *shader;
    Shader *skyboxShader;
    GLuint cubemapTexture;
	GLuint blockTexture;

    GLuint loadCubemap(vector<const GLchar*> faces) {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glActiveTexture(GL_TEXTURE0);
        
        int width, height;
        unsigned char* image;
        
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        for(GLuint i = 0; i < faces.size(); i++)
        {
            image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
            glTexImage2D(
                         GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                         GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
                         );
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        
        return textureID;
    }
    
    void init() {
        shader->enable();
        blockShape = ShapeGenerator::makeCube();
        glGenVertexArrays(1, &blockVAO);
        glBindVertexArray(blockVAO);
        
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, blockShape.vertexBufferSize(), blockShape.vertices, GL_STATIC_DRAW);
        //vbo 0 vertex - vec3 position, vec3 normal, vec2 texture cordinates
    
        //Vertex Atrributes
        //vertex position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);
        
        //vertex normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (char*)(sizeof(GLfloat) * 3));
        
        //vertex texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (char*)(sizeof(GLfloat) * 6));
        
        GLuint indexBufferID;
        glGenBuffers(1, &indexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, blockShape.indexBufferSize(), blockShape.indices, GL_STATIC_DRAW);
		shader->disable();
		
        /////////  SKYBOX  //////////
        skyboxShader->enable();
        
        glGenVertexArrays(1, &skyboxVAO);
        glBindVertexArray(skyboxVAO);
        
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, blockShape.vertexBufferSize(), blockShape.vertices, GL_STATIC_DRAW);
        //vbo 0 vertex - vec3 position, vec3 normal, vec2 texture cordinates
        
        //Vertex Atrributes
        //vertex position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);
        
        
        glGenBuffers(1, &indexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, blockShape.indexBufferSize(), blockShape.indices, GL_STATIC_DRAW);
        
        //
        
        vector<const GLchar*> faces;

        faces.push_back("/Users/isaiah/Documents/computer science/Game Engine/src/assets/lake skybox/lake1_ft.jpg");
        faces.push_back("/Users/isaiah/Documents/computer science/OpenGL/OpenGL/assets/lake skybox/lake1_bk.jpg");
        
        faces.push_back("/Users/isaiah/Documents/computer science/Game Engine/src/assets/lake skybox/lake1_up.jpg");
        faces.push_back("/Users/isaiah/Documents/computer science/Game Engine/src/assets/lake skybox/lake1_dn.jpg");
        
        faces.push_back("/Users/isaiah/Documents/computer science/Game Engine/src/assets/lake skybox/lake1_rt.jpg");
        faces.push_back("/Users/isaiah/Documents/computer science/Game Engine/src/assets/lake skybox/lake1_lf.jpg");
        

        vector<const GLchar*> faces2;
        faces2.push_back("/Users/isaiah/Documents/computer science/Game Engine/src/assets/skybox2/lf.png");
        faces2.push_back("/Users/isaiah/Documents/computer science/Game Engine/src/assets/skybox2/rt.png");

        
        faces2.push_back("/Users/isaiah/Documents/computer science/Game Engine/src/assets/skybox2/up.png");
        faces2.push_back("/Users/isaiah/Documents/computer science/Game Engine/src/assets/skybox2/dn.png");
        
        faces2.push_back("/Users/isaiah/Documents/computer science/Game Engine/src/assets/skybox2/ft.png");
        faces2.push_back("/Users/isaiah/Documents/computer science/Game Engine/src/assets/skybox2/bk.png");

        cubemapTexture = loadCubemap(faces2);
		skyboxShader->disable();
    }
    
    public:
    Renderer(Shader *shader, Shader *skyboxShader) {
        this->shader = shader;
        this->skyboxShader = skyboxShader;
        init();
    }
    
    void render(vector<Block*> *blocks) {
        shader->enable();
  
		glBindTexture(GL_TEXTURE_2D, blockTexture);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(blockVAO);

        
        for (int i = 0; i < blocks->size(); i++) {
            shader->setUniformMat4("instanceMatrix", blocks->at(i)->getModelMatrix());
			shader->setUniform3f("scale", blocks->at(i)->getSize());

            glDrawElements(GL_TRIANGLES, blockShape.numIndices, GL_UNSIGNED_SHORT, 0);
        }
		shader->disable();
    }
    
    void renderSkybox() {
        skyboxShader->enable();
        glDepthMask(GL_FALSE);
        glFrontFace(GL_CCW);
		glCullFace(GL_FRONT);

        
        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawElements(GL_TRIANGLES, blockShape.numIndices, GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);
        
        glFrontFace(GL_CW);
        glDepthMask(GL_TRUE);
		skyboxShader->disable();
    }
	
	void setBlockTexture(GLuint blockTexture) {
		this->blockTexture = blockTexture;
	}
};