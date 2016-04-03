#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <stdio.h>
#include <math.h>
#include "Block.h"

class BlockCam {
	float rotation;
	float elevation;
	Block *block;
	
	float maxElevation;
	float minElevation;
	vec2 lastMousePosition;
	vec2 sensitivity;

public:
	BlockCam(Block *block);
	
	void update(float delta);
	void update(vec2 &mousePosition);
	void move(const vec3 &moveVector, vec3 &position);
	vec3 getLookVector() { return block->getPosition() - getPosition();}
	vec3 getSideVector() { return glm::cross(getLookVector(), vec3(0,-1,0));}

	vec2 getDelta(vec2 &mousePosition);
	vec3 getPosition() const;
	mat4 getWorldToViewMatrix() const;
	mat4 getViewMatrix() const;
	float getRotation() {return rotation;}
	
	void setLastMousePosition(const vec2 &mousePosition) {lastMousePosition = mousePosition;}
	
};