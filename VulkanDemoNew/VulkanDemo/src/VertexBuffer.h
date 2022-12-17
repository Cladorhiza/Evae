#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

#include "VkHelpers.h"
#include "BasicVertex.h"
class VertexBuffer
{
private:
	VkDevice device;
	VkBuffer buffer;
	VkDeviceMemory bufferMemory;


public:
	inline VkBuffer GetBuffer() { return buffer; }
	inline VkDeviceMemory GetBufferMemory() { return bufferMemory; }
	void Init(VkDevice dev, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, std::vector<BasicVertex>& vertices);
	void Destroy();
};

