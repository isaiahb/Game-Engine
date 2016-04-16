#pragma once

#include "ShapeData.h"
#include "ShapeGenerator.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <btBulletDynamicsCommon.h>

typedef glm::vec3 Color;

using namespace glm;
using namespace std;

class Block {
private:
    vec3 position;
    vec3 size;
    vec3 rotation;
    vec3 color;
	vec3 outlineColor;
	
    float transparency;
    
    mat4 scaleMatrix;
    mat4 rotationMatrix;
    mat4 translationMatrix;
    mat4 fullMatrix;
	btRigidBody *body;
    
    void updateFullMatrix() {
        fullMatrix = translationMatrix * scaleMatrix;
    }
    
public:
    static vector<Block*> blocks;
    
    Block(float x = 0, float y = 0, float z = 0, float w = 1, float h = 1, float l = 1) {
        setPosition(x, y, z, false);
        setSize(w, h, l);
        //setRotation(0, 30, 0, false);
		color = vec3(0.2f, 0.3f, 0.5f);//vec3(0.2, 0.3, 0.5);
		outlineColor = vec3(0, 0, 0);
        blocks.push_back(this);
    }
	void updatePhysicsPosition() {
		body->getWorldTransform().setOrigin(btVector3(position.x, position.y, position.z));
	}
	void updatePhysicsRotation() {
		btQuaternion rotate(rotation.x, rotation.y, rotation.z);
		//body->getWorldTransform().setRotation(rotate);
	}
	void updatePhysicsSize() {}
	
	void setPosition(const vec3 vec, bool updateBody = true) {
		position = vec3(vec);
		translationMatrix = glm::translate(mat4(), position);
		updateFullMatrix();
		if (updateBody) updatePhysicsPosition();
	}
    void setPosition(float x = 0, float y = 0, float z = 0, bool updateBody = true) {
        position = vec3(x, y, z);
        translationMatrix = glm::translate(mat4(), position);
        updateFullMatrix();
		if (updateBody) updatePhysicsPosition();

    }
    void setSize(float x = 0, float y = 0, float z = 0) {
        size = vec3(x, y, z);
        scaleMatrix = glm::scale(mat4(), size);
        updateFullMatrix();
    }
	
    void setRotation(float x = 0, float y = 0, float z = 0, bool updateBody = true) {
        rotation = vec3(x, y, z);
        rotationMatrix = mat4();
        rotationMatrix = glm::rotate(rotationMatrix, rotation.x, (vec3(1, 0, 0)));
        rotationMatrix = glm::rotate(rotationMatrix, rotation.y, (vec3(0, 1, 0)));
        rotationMatrix = glm::rotate(rotationMatrix, rotation.z, (vec3(0, 0, 1)));
        updateFullMatrix();
		if (updateBody) updatePhysicsRotation();

    }
	void setRotation(const float &angle, const vec3 &axis, bool updateBody = true) {
		rotationMatrix = glm::rotate(mat4(), angle, axis);
		updateFullMatrix();
		if (updateBody) updatePhysicsRotation();
	}
	
	void setBody(btRigidBody *body) {this->body = body;}
    vec3 getPosition() {return vec3(position);}
    vec3 getRotation() {return vec3(rotation);}
    vec3 getSize() {return vec3(size);}
	Color getColor(){return vec3(color);}
	Color getOutlineColor(){return vec3(outlineColor);}
	
	btRigidBody* getBody() {return body;}

    mat4 getModelMatrix() {return fullMatrix;}
	mat4 getModelMatrix(float scale) {
		vec3 scaleSize = vec3(size);
		vec3 one(1,1,1);
		scaleSize += (one * scale) - one;
		return translationMatrix * glm::scale(mat4(), scaleSize);
	}
};
