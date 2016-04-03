//
//  Camera.cpp
//  Game Engine
//
//  Created by isaiah ballah on 2016-03-24.
//  Copyright © 2016 isaiah ballah. All rights reserved.
//

#include "FreeCam.h"
#include <iostream>

Camera::Camera() : viewDirection(0.0f,0.0f,-1.0f), up(0.0f,-1.0f,0.0f), lastMousePosition(0.0f,0.0f) ,realUp(up){}
mat4 Camera::getWorldToViewMatrix() const {
    return glm::lookAt(position, position + viewDirection, up);
}
mat4 Camera::getViewMatrix() const {
    return glm::lookAt(vec3(0, 0, 0), viewDirection, up);
}

void Camera::move(const vec3 &moveVector, vec3 &position) {
	vec3 move(moveVector * 0.1f);
    position += viewDirection * move.z;
    position += up * move.y;
    vec3 side = glm::normalize(getSideVector());
    position += side * move.x;
}

void Camera::update(vec2 &mousePosition) {
    vec2 delta =  getDelta(mousePosition);
    
    float max = 5;
    if (delta.length() >= max) {
        delta.x = 0;
        delta.y = 0;
    }
    delta *= 0.005;
    viewDirection = glm::mat3(glm::rotate(delta.x, up)) * viewDirection;
    vec3 toRotateAround = glm::cross(viewDirection, up);
    viewDirection = glm::mat3(glm::rotate(delta.y, toRotateAround)) * viewDirection;
    realUp = glm::cross(toRotateAround, viewDirection);

    lastMousePosition = mousePosition;
}

void Camera::reset() {
    position = vec3(0,0,0);
    viewDirection = vec3(0.0f,0.0f,-1.0f);
    up = vec3(0.0f,-1.0f,0.0f);
}