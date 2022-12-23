#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "VkHelpers.h"

#include <stdexcept>
class Surface
{
private:
	VkInstance instance;
	VkSurfaceKHR surface;

public:
	Surface(VkInstance inst, GLFWwindow* window);
	~Surface();

	VkSurfaceKHR GetSurface() { return surface; }
};

