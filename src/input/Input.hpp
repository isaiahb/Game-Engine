#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <iostream>
#include <vector>

typedef void (*MouseMoved)(glm::vec2 position, glm::vec2 delta);
typedef void (*MouseClicked)(glm::vec2 position);
typedef void (*MouseScrolled)(glm::vec2 delta);

typedef void (*KeyPressed)();
typedef void (*KeyReleased)();



typedef std::vector<MouseMoved> MouseMovedFunctions;
typedef std::vector<MouseClicked> MousePressedFunctions;
typedef std::vector<MouseClicked> MouseReleasedFunctions;
typedef std::vector<MouseScrolled> MouseScrolledFunctions;

typedef std::vector<KeyPressed>	KeyPressedFunctions;
typedef std::vector<KeyReleased> KeyReleasedFunctions;


using namespace std;

class Input {
	
	void init();
	
public:
	void update();
	
	struct Mouse {
		glm::vec2 position;
		bool mouseButtonDown[256];
		void addMouseMovedListener(MouseMoved func);
		void addMousePressedListener(int button, MouseClicked func);
		void addMouseReleasedListener(int button, MouseClicked func);
		void addMouseScrolledListener(MouseClicked func);

		
		MouseMovedFunctions   mouseMovedListeners;
		MouseScrolledFunctions   mouseScrolledListeners;
		MousePressedFunctions mousePressedListeners[256];
		MouseReleasedFunctions mouseReleasedListeners[256];
	} mouse;
	
	struct Keys {
		int numKeys = 256;
		bool keyDown[256];
		void addKeyPressedListener(int keycode, KeyPressed);
		void addKeyReleasedListener(int keycode, KeyReleased);
		
		KeyPressedFunctions	keyPressedListeners[1024];
		KeyReleasedFunctions keyReleasedListeners[1024];
	} keys;
};