#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "VkHelpers.h"

#include <iostream>
class Device
{
private:

	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

public:

	Device(VkPhysicalDevice physicalDev, VkSurfaceKHR surface, const bool enableValidationLayers,
			const std::vector<const char*>& deviceExtensions,
			const std::vector<const char*>& validationLayers);
	~Device();

	VkDevice GetDevice() { return device; };
	VkQueue GetGraphicsQueue() { return graphicsQueue; }
	VkQueue GetPresentQueue() { return presentQueue; }

};

