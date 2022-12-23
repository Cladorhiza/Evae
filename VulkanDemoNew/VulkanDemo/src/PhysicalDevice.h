#pragma once
#include "VkHelpers.h"
#include "SwapChain.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <stdexcept>

class PhysicalDevice
{
private:

	VkPhysicalDevice physicalDevice;

public:

	PhysicalDevice(VkInstance instance, VkSurfaceKHR surface, std::vector<const char*> deviceExtensions);

	VkPhysicalDevice GetPhysicalDevice() { return physicalDevice; }

	bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<const char*> deviceExtensions);


};

