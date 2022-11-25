#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

#include "VkHelpers.h"
#include "Vertex.h"
class IndexBuffer
{
    VkDevice device;
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;

public:
    inline VkBuffer GetBuffer() { return buffer; };
    void Init(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, std::vector<uint16_t> indices);
    void Destroy();
};

