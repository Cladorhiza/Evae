#pragma once
#include "VkHelpers.h"
#include "SwapChain.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <iostream>

class RenderPass
{
private:

	VkDevice device;
	VkRenderPass renderPass;

public:

	RenderPass(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	~RenderPass();

	VkRenderPass GetRenderPass() { return renderPass; }
};

