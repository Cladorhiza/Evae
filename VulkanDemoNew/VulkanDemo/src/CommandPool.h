#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <unordered_map>
#include <stdexcept>

#include "VkHelpers.h"

class CommandPool
{
private:

	VkDevice device;
	VkPhysicalDevice physicalDevice;
	VkCommandPool commandPool;
	std::unordered_map<int, std::vector<VkCommandBuffer>> commandBufferMap;

public:

	CommandPool(VkDevice dev, VkPhysicalDevice physicalDev, VkSurfaceKHR surface);
	~CommandPool();

	VkCommandPool GetCommandPool() { return commandPool; }
	std::vector<VkCommandBuffer> GetBuffers(int id) { return commandBufferMap[id]; }
	
	void AddCommandBuffer(int id, uint32_t count, VkCommandBufferLevel buffLevel);
};

