//
//  BlockCam.cpp
//  Game Engine
//
//  Created by isaiah ballah on 2016-03-29.
//  Copyright © 2016 isaiah ballah. All rights reserved.
//

#include "BlockCam.hpp"
#define PI 3.14159
float rad(float degrees) {
	float radians = ( degrees * PI ) / 180;
	return radians;
}

float deg(float radians) {
	float degrees = ( radians * 180 ) / PI;
	return degrees;
}

BlockCam::BlockCam(Block *block) {
	this->block = block;
	rotation = 0;
	elevation = rad(0);
	maxElevation = rad(89);//rad(70);
	minElevation = rad(-89);
	sensitivity = vec2(0.01, 0.01);
}

void BlockCam::update(vec2 &mousePosition) {
	vec2 delta = getDelta(mousePosition);
	elevation += delta.y * sensitivity.y;
	rotation += delta.x * sensitivity.x;
	
	elevation = clamp(elevation, minElevation, maxElevation);
//	int rotationInt = (int)rotation;
	//float extra = rotation - rotationInt;
	//rotation = (rotationInt % 360);
	//rotation += extra;
//	std::cout<<"elevation "<<elevation<<std::endl;
//	std::cout<<"rotation "<<rotation<<std::endl;

	lastMousePosition  = mousePosition;
}

vec2 BlockCam::getDelta(vec2 &mousePosition) {
	return mousePosition - lastMousePosition;
}

vec3 BlockCam::getPosition() const {
	float radius = 15;
	vec3 position;
	
	float a;	//magic variable
	//polar = math.rad(self.polar)
	//elevation = math.rad(self.elevation)
	//radius = self.radius
	a = radius * cos(elevation);
	position.x = a * cos(rotation);
	position.y = radius * sin(elevation);
	position.z = a * sin(rotation);
	return position + block->getPosition();
}
void BlockCam::move(const vec3 &moveVector, vec3 &position) {
	float angle = (rotation - PI / 2.0f);
	vec3 axis(0, 1, 0);
	mat4 rotate = glm::rotate(mat4(), angle, axis);
	mat4 translate = glm::translate(mat4(), moveVector);
	
	mat4 t = translate * rotate;
	position = vec3(vec4(position,1) * t);
	this->t = translate * rotate;
}


mat4 BlockCam::getWorldToViewMatrix() const {
	return glm::lookAt(getPosition(), block->getPosition(), vec3(0,-1,0));
}
mat4 BlockCam::getViewMatrix() const {
	return glm::lookAt(getPosition(), block->getPosition(), vec3(0,-1,0));
}