//
//  Input.cpp
//  Game Engine
//
//  Created by isaiah ballah on 2016-04-07.
//  Copyright © 2016 isaiah ballah. All rights reserved.
//
#include "Input.hpp"
void mouseButtonFunction(GLFWwindow*, int, int, int);
void mouseScrollFunction(GLFWwindow*, double, double);

void Input::init() {
	mouse.mouseMovedListeners = vector<MouseMoved>();
	for (int i = 0; i < 256; i++) {
		mouse.mousePressedListeners[i] = MousePressedFunctions();
		mouse.mouseReleasedListeners[i] = MouseReleasedFunctions();
		mouse.mouseButtonDown[i] = false;
	}
	for (int i = 0; i < 1024; i++) {
		keys.keyPressedListeners[i] = KeyPressedFunctions();
		keys.keyReleasedListeners[i] = KeyReleasedFunctions();
		keys.keyDown[i] = false;
	}
//	int i = 5;
	
//	GLFWmousebuttonfun foo = [](GLFWwindow *window, int button, int action, int mods){};


}
void update() {
	
}

void Input::Mouse::addMouseMovedListener(MouseMoved func) {
	mouseMovedListeners.push_back(func);
}
void Input::Mouse::addMousePressedListener(int button, MouseClicked func) {
	mousePressedListeners[button].push_back(func);
}
void Input::Mouse::addMouseReleasedListener(int button, MouseClicked func) {
	mouseReleasedListeners[button].push_back(func);
}

void Input::Keys::addKeyPressedListener(int keycode, KeyPressed func) {
	keyPressedListeners[keycode].push_back(func);

}
void Input::Keys::addKeyReleasedListener(int keycode, KeyReleased func) {
	keyReleasedListeners[keycode].push_back(func);
}




void mouseButtonFunction(GLFWwindow *window, int button, int action, int mods){};
void mouseScrollFunction(GLFWwindow *window, double detlaX, double deltaY){};


