#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "VkHelpers.h"

class UniformBuffer
{
private:
    VkDevice device;
    VkPhysicalDevice physicalDevice;

    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
    void* bufferMapped;
    

public:
    inline VkBuffer GetBuffer() { return buffer; }
    inline void* GetBufferMappedMemory() { return bufferMapped; }
    void Init(uint32_t bufferSize, VkDevice dev, VkPhysicalDevice physicalDev);
    void Destroy();
};

