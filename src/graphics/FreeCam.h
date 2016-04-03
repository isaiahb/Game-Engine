#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <stdio.h>

using glm::vec3;
using glm::vec2;
using glm::mat4;

class Camera {
    vec3 viewDirection;
    vec3 up;
    vec3 realUp;
    
public:
    vec2 lastMousePosition;
    vec3 position;
    Camera();
    mat4 getWorldToViewMatrix() const;
    mat4 getViewMatrix() const;

    void lookAt(vec3 vec);
    void lookAt(float x, float y, float z);
    void update(vec2 &mousePosition);
    void update(float, float);
    void move(const vec3 &moveVector, vec3 &position);
    void reset();
    void resetLastPosition(vec2 &mousePosition) {
        lastMousePosition = mousePosition;

    }
    vec2 getDelta(vec2 &mousePosition) {return mousePosition - lastMousePosition;}
    vec3 getLookVector() { return viewDirection;}
    vec3 getUpVector() { return realUp;}
    vec3 getSideVector() { return glm::cross(viewDirection, realUp);}
	vec3 getPosition(){ return position;}
    
};




