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

	inline VkCommandPool GetCommandPool() { return commandPool; }
	inline std::vector<VkCommandBuffer> GetBuffers(int id) { return commandBufferMap[id]; }
	void Init(VkDevice dev, VkPhysicalDevice physicalDev, VkSurfaceKHR surface);
	void AddCommandBuffer(int id, uint32_t count, VkCommandBufferLevel buffLevel);
	void Destroy();
};

