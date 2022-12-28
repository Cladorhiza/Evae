#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

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

    UniformBuffer(uint32_t bufferSize, VkDevice dev, VkPhysicalDevice physicalDev);
    ~UniformBuffer();

    VkBuffer GetBuffer() { return buffer; }
    void* GetBufferMappedMemory() { return bufferMapped; }
};

