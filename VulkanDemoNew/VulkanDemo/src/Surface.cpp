#include "Surface.h"

Surface::Surface(VkInstance inst, GLFWwindow* window) 
	:instance(inst)
{

	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}


Surface::~Surface() {

	vkDestroySurfaceKHR(instance, surface, nullptr);
}