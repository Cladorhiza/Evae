#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

#include "VkHelpers.h"
#include "Vertex.h"
class VertexBuffer
{
private:
	VkDevice device;
	VkBuffer buffer;
	VkDeviceMemory bufferMemory;


public:

	//getters
	VkBuffer GetBuffer() { return buffer; }
	VkDeviceMemory GetBufferMemory() { return bufferMemory; }
	
	VertexBuffer(VkDevice dev, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, std::vector<Vertex>& vertices);
	~VertexBuffer();
};

