#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <stdio.h>

class Camera {
public:
	vec2 lastMousePosition;
	vec3 position;

	mat4 getWorldToViewMatrix() const;
	mat4 getViewMatrix() const;
	
	void update(vec2 &mousePosition);
	void update(float, float);
	void move(vec3);
	
	vec2 getDelta(vec2 &mousePosition) {return mousePosition - lastMousePosition;}
	vec3 getLookVector() { return viewDirection;}
	vec3 getUpVector() { return realUp;}
	vec3 getSideVector() { return glm::cross(viewDirection, realUp);}
	vec3 getPosition(){ return position;}
	
};