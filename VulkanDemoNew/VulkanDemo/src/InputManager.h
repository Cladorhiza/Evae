#pragma once
#include "Window.h"

#include <GLFW/glfw3.h>

#include <unordered_map>


class InputManager
{
private:

	Window window;
	GLFWwindow* pWindow;
	
	//all keycodes that need to be tracked
	const std::vector<int> usedKeys{
		GLFW_KEY_W,
		GLFW_KEY_A,
		GLFW_KEY_S,
		GLFW_KEY_D,
		GLFW_KEY_Q,
		GLFW_KEY_E,
		GLFW_KEY_T,
		GLFW_KEY_ESCAPE,
		GLFW_KEY_RIGHT,
		GLFW_KEY_LEFT,
		GLFW_KEY_UP,
		GLFW_KEY_DOWN
	};
	//stored state of keys
	std::unordered_map<int, int> keys;
	//has key toggled this frame
	std::unordered_map<int, bool> keyToggle;
	//mouse position
	double cursorXPos = 0;
	double cursorYPos = 0;
	//mouse position last frame
	double cursorOldXPos = 0;
	double cursorOldYPos = 0;
	//how many times the mouse has scrolled positively/negatively away from neutral
	double scrollYAxisOffset = 0;

public:

	InputManager();
	InputManager(Window& GLFWwindow);

	int GetKeyState(int GLFWKeyCode) { return keys[GLFWKeyCode]; }
	bool GetKeyToggle(int GLFWKeyCode) { return keyToggle[GLFWKeyCode]; }
	float GetMouseX() { return (float)cursorXPos; }
	float GetMouseY() { return (float)cursorYPos; }
	float GetMouseScrollYOffset() { return (float)scrollYAxisOffset; }
	float GetDeltaMouseX() { return (float)(cursorXPos - cursorOldXPos); }
	float GetDeltaMouseY() { return (float)(cursorYPos - cursorOldYPos); }

	void Init(Window& GLFWwindow);
	void Poll();

};