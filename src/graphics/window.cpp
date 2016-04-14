//
//  Window.cpp
//  OpenGL
//
//  Created by isaiah ballah on 2016-03-19.
//  Copyright © 2016 isaiah ballah. All rights reserved.
//

#include "Window.hpp"
vec2 total;
vec2 lastTotal;

namespace ballah { namespace graphics {
    
    ////////////////////////////////////////////////////////////////////////////////////
    void window_resize_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->size = vec2(width, height);
    }
	
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        Window* win = (Window*)glfwGetWindowUserPointer(window);
        win->keys[key] = action != GLFW_RELEASE;
		win->getInput()->keys.keyDown[key]  = action != GLFW_RELEASE;
		if (action != GLFW_RELEASE) {
			KeyPressedFunctions funcs = win->getInput()->keys.keyPressedListeners[key];
			for (int i = 0; i < funcs.size(); i++) {
				funcs[i]();
			}
		}
		else {
			KeyReleasedFunctions funcs = win->getInput()->keys.keyReleasedListeners[key];
			for (int i = 0; i < funcs.size(); i++) {
				funcs[i]();
			}
		}
	

    }
    
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
        Window* win = (Window*)glfwGetWindowUserPointer(window);
        win->mouseButtons[button] = action != GLFW_RELEASE;
		win->getInput()->mouse.mouseButtonDown[button] =  action != GLFW_RELEASE;
		int x, y;
		if (action != GLFW_RELEASE) {
			MousePressedFunctions funcs = win->getInput()->mouse.mousePressedListeners[button];
			for (int i = 0; i < funcs.size(); i++) {
				funcs[i](glm::vec2(x, y));
			}
		}
		else {
			MouseReleasedFunctions funcs = win->getInput()->mouse.mouseReleasedListeners[button];
			for (int i = 0; i < funcs.size(); i++) {
				funcs[i](glm::vec2(x, y));
			}
		}
    }

    
    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
        Window* win = (Window*)glfwGetWindowUserPointer(window);
        win->mouse = vec2(xpos, ypos);
        win->delta = win->mouse - win->last;
        win->last = win->mouse;
		win->getInput()->mouse.position = vec2(xpos, ypos);

    }
    ////////////////////////////////////////////////////////////////////////////////////
    
    
    Window::Window(int width, int height, const char *title) {
        this->width = width;
        this->height = height;
        this->title = title;
        mouse = last = vec2(width/2.0f, height/2.0f);
        if(!init()) {
            glfwTerminate();
        }
        
        for (int i = 0; i < MAX_KEYS; i++)
        {
            keys[i] = false;
        }
        
        for (int i = 0; i < MAX_BUTTONS; i++)
        {
            mouseButtons[i] = false;
        }
    }
    Window::~Window(){
        glfwTerminate();
    }

    void Window::setClearColor(float r = 0, float g = 0, float b = 0, float a = 1) const{
        glClearColor(r, g, b, a);
    }
    void Window::clear() const{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void Window::update() {
        glfwGetFramebufferSize(window, &width, &height);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    bool Window::isRunning() {
        return !glfwWindowShouldClose(window);
    }
    
    bool Window::init() {
        if(!glfwInit()) {
            std::cout<<"Failed to initialized glfw"<<std::endl;
            return false;
        }
        else {
            std::cout<<"openGL initialized succesfully"<<std::endl;
        }
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        window = glfwCreateWindow(width, height, title, NULL, NULL);
        if(!window) {
            glfwTerminate();
            std::cout<<"Failed to create GLFW window"<<std::endl;
            return false;
        }
		
        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, this);
        glfwSetWindowSizeCallback(window, window_resize_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetCursorPos(window, mouse.x, mouse.y);
        
        glewExperimental = GL_TRUE;
        if(glewInit() != GLEW_OK) {
            std::cout<<"i cri evri tiem"<<std::endl;
            std::cout<<"gleu not ok"<<std::endl;
            return false;

        }
        std::cout<<"OpenGL "<<glGetString(GL_VERSION)<<std::endl;
        return true;
    }
    bool Window::isKeyPressed(unsigned int keycode) const {
        
        // TODO: Log this!
        if (keycode >= MAX_KEYS)
            return false;
        
        return keys[keycode];
    }
    
    bool Window::isMouseButtonPressed(unsigned int button) const
    {
        // TODO: Log this!
        if (button >= MAX_BUTTONS)
            return false;
        
        return mouseButtons[button];
    }
    
    void Window::getMousePosition(double& x, double& y) const
    {
        x = mouse.x;
        y = mouse.y;
    }
    
    int Window::getWidth() const {
        return width;
    }
    
    int Window::getHeight() const {
        return height;
    }
    

}}