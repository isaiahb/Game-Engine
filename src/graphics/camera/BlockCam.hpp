#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <stdio.h>
#include <math.h>
#include "Block.h"

class BlockCam {
	float rotation;
	float elevation;
	float radius = 10;
	float minRadius = 3;
	float maxRadius = 20;

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
	mat4 t;
	float getRotation() {return rotation;}
	void addRadius(float value) {
		radius += value;
		radius = radius > maxRadius ? maxRadius : radius;
		radius = radius < minRadius ? minRadius : radius;

	}
	void addRotation(float value) {
		rotation += value;
	}
	
	void setLastMousePosition(const vec2 &mousePosition) {lastMousePosition = mousePosition;}
	
};