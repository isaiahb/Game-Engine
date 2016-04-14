//
//  Window.hpp
//  OpenGL
//
//  Created by isaiah ballah on 2016-03-19.
//  Copyright © 2016 isaiah ballah. All rights reserved.
//
#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "FreeCam.h"
#include "Input.hpp"


namespace ballah { namespace graphics {
#define MAX_KEYS	1024
#define MAX_BUTTONS	32
    using glm::vec2;
    
    class Window {
    private:
        const char *title;
        int width, height;
        GLFWwindow *window;
		Input input;
        
      public:
        vec2 mouse;
        vec2 last;
        vec2 delta;
		vec2 size;
        bool keys[MAX_KEYS];
        bool mouseButtons[MAX_BUTTONS];
        bool isRunning();
        
        Window(int x, int y,const char *title);
        ~Window();
        void setClearColor(float, float, float, float) const;
        void clear() const;
        void update();
        
        int getWidth() const;
        int getHeight() const;
        
        bool isKeyPressed(unsigned int keycode) const;
        bool isMouseButtonPressed(unsigned int button) const;
        void getMousePosition(double& x, double& y) const;
		Input* getInput() {return &input;}
        
        GLFWwindow* getWindow() {return window;}
    private:
        bool init();
    };
}}

