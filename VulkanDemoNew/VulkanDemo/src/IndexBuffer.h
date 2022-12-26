#pragma once

#include "VkHelpers.h"
#include "Vertex.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

class IndexBuffer
{
    VkDevice device;
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;

public:
    //constructors
    IndexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, const std::vector<uint32_t>& indices);
    ~IndexBuffer();
    
    //getters
    inline VkBuffer GetBuffer() { return buffer; };
};

