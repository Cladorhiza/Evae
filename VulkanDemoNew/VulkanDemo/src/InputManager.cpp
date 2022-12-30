#include "InputManager.h"


//void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	scrollYAxisOffset += yoffset;
//}
void InputManager::Init(GLFWwindow* window) {
	//glfwSetScrollCallback(window, ScrollCallback);
}

void InputManager::Poll(GLFWwindow* window) {
	//updates glfw keys
	glfwPollEvents();

	//updates state of keys

	for (int keyName : usedKeys) {
		int state = glfwGetKey(window, keyName);
		if (state != keys[keyName] && keys[keyName] == GLFW_RELEASE) keyToggle[keyName] = true;
		else keyToggle[keyName] = false;
		keys[keyName] = state;
	}
	keys[GLFW_MOUSE_BUTTON_LEFT] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	keys[GLFW_MOUSE_BUTTON_RIGHT] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	//update cursor position
	cursorOldXPos = cursorXPos;
	cursorOldYPos = cursorYPos;
	glfwGetCursorPos(window, &cursorXPos, &cursorYPos);

}