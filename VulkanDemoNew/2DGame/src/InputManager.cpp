#include "InputManager.h"

InputManager::InputManager() 
	:window(), pWindow(nullptr)
{


}

InputManager::InputManager(Window& GLFWwindow) {

	Init(GLFWwindow);

}

void InputManager::Init(Window& GLFWwindow) {
	window = GLFWwindow;
	pWindow = GLFWwindow.GetWindow();
}

void InputManager::Poll() {
	scrollYAxisOffset = window.GetScrollYOffSet();

	//updates state of keys
	for (int keyName : usedKeys) {
		int state = glfwGetKey(pWindow, keyName);
		if (state != keys[keyName] && keys[keyName] == GLFW_RELEASE) keyToggle[keyName] = true;
		else keyToggle[keyName] = false;
		keys[keyName] = state;
	}
	keys[GLFW_MOUSE_BUTTON_LEFT] = glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_LEFT);
	keys[GLFW_MOUSE_BUTTON_RIGHT] = glfwGetMouseButton(pWindow, GLFW_MOUSE_BUTTON_RIGHT);

	//update cursor position
	cursorOldXPos = cursorXPos;
	cursorOldYPos = cursorYPos;
	glfwGetCursorPos(pWindow, &cursorXPos, &cursorYPos);

}